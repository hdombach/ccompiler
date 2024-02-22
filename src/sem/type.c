#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "type.h"
#include "../util/log.h"
#include "scope.h"
#include "../ast/declaration.h"
#include "sprim.h"
#include "sarray.h"
#include "scompound.h"
#include "spointer.h"
#include "sfunction.h"
#include "stypedef.h"
#include "senum.h"

/*************************************************************
 * Semantic Type
 *************************************************************/

void initSType(SType *type, STypeVTable *vtable) {
	type->vtable = vtable;
	type->type = STT_UNKNOWN;
	type->isConst = 0;
	type->isVolatile = 0;
	type->isTypedef = 0;
	type->isExtern = 0;
}

void destroySType(SType *type) {
	if (type->vtable->table.freeFunc) {
		type->vtable->table.freeFunc(type);
	}
	type->type = STT_UNKNOWN;
}

int loadDecl(
		SType *type,
		SType *internal,
		ASTNode *node,
		ASTScope *scope)
{
	// Anonymous type 
	if (!node) {
		mvSType(type, internal);
		return 1;
	}
	switch (node->type) {
		{
			ASTDeclarator *decl = (ASTDeclarator *) node;
			int res = loadDecl(
					type, 
					internal, 
					decl->encl, 
					scope);
			if (res) {
				stypeLoadTypeQualifier(type, decl->qualifiers);
			}
			return res;
		}
		case AST_DECLARATOR: return loadDecl(
			 type,
			 internal,
			 ((ASTDeclarator *) node)->encl,
			 scope);
		case AST_POINTER_DECL: return loadSPointer(
			type,
			internal,
			(ASTDeclarator *) node,
			scope);
		case AST_ARRAY_DECL: return loadSArray(
			(SArray *) type,
			internal,
			(ASTArrayDecl *) node,
			scope);
		case AST_FUNC_DECL: return loadSFunction(
			type,
			internal,
			(ASTFuncDecl *) node,
			scope);
		case AST_IDENTIFIER_DECL:
				mvSType(type, internal);
				return 1;
		default: return 0;
	};
};

static int loadTypespec(SType *type, ASTTypeSpec *spec, ASTScope *scope) {
	if (!ASSERT(spec->node.type == AST_TYPE_SPEC)) return 0;

	switch (spec->typeSpecType) {
		case AST_TST_UNION:
		case AST_TST_STRUCT:
			return loadSCompoundRef(
				(SCompoundRef *) type,
				(ASTStructDecl *) spec->content,
				scope);
		case AST_TST_ENUM:
			return loadSEnumRef(
					(SEnumRef *) type, 
					(ASTEnumDecl *) spec->content, 
					scope);
		case AST_TST_VOID:
			type->type = STT_VOID;
			return 1;
		case AST_TST_ARITH:
			return loadSPrim((SPrim *) type, spec);
		case AST_TST_TYPEDEF:
			return loadSTypedef(
				(STypedef *) type,
				(ASTIdentifier *) spec->content,
				scope);
		case AST_TST_UNKNOWN:
			return 0;
	}
}

int loadSTypes(ASTScope *scope, ASTDeclaration *declaration) {
	if (!ASSERT(declaration->node.type == AST_DECLARATION)) return 0;

	STypeBuf tempInternal, tempType;

	if (!loadTypespec((SType *) &tempInternal, declaration->typeSpec, scope)) {
		logCerr(CERR_UNKNOWN, declaration->node.tok, "Problem loading typespec");
		return 0;
	}

	for (int i = 0; i < declaration->declarators.size; i++) {
		ASTDeclarator *declarator = dlistGetm(&declaration->declarators, i);
		/*technically, loadDecl takes ownership of internal. However
		 * since typespec types are trivially copiable, tis fine */

		if (!loadDecl(
				(SType *) &tempType,
				(SType *) &tempInternal,
				(ASTNode *) declarator,
				scope))
		{
			logCerr(
					CERR_UNKNOWN,
					declaration->node.tok,
					"Problem loading type identifier %s",
					astDeclaratorName((ASTNode *) declarator));
			return 0;
		}

		stypeLoadStorageClass((SType *) &tempType, declaration->typeSpec->storage);

		if (!astScopeAddIdent(
				scope,
				(SType *) &tempType,
				strdup(astDeclaratorName((ASTNode *) declarator))))
		{
			logCerr(
					CERR_TYPE,
					declaration->node.tok,
					"Couldn't add identifier %s",
					astDeclaratorName((ASTNode *) declarator));
			return 0;
		}
	}

	return 1;
}

int loadParamSType(ASTScope *scope, struct ASTParam *param) {
	if (!ASSERT(param->node.type == AST_PARAM)) return 0;

	STypeBuf tempInternal, tempType;

	if (!loadTypespec((SType *) &tempInternal, param->typeSpec, scope)) {
		logCerr(CERR_UNKNOWN, param->node.tok, "Problem loading param typespec");
		return 0;
	}

	if (!loadDecl(
			(SType *) &tempType,
			(SType *) &tempInternal,
			(ASTNode *) param->declarator,
			scope))
	{
		logCerr(
				CERR_UNKNOWN,
				param->node.tok,
				"Problem loading param identifier %s",
				astDeclaratorName((ASTNode *) param->declarator));
		return 0;
	}

	const char *name = astDeclaratorName((ASTNode *) param->declarator);
	if (name) {
		if (!astScopeAddIdent(
				scope, 
				(SType *) &tempType, 
				strdup(astDeclaratorName((ASTNode *) param->declarator))))
		{
			logCerr(
					CERR_UNKNOWN,
					param->declarator->node.tok,
					"Couldn't add param identifier %s",
					astDeclaratorName((ASTNode *) param->declarator));
			return 0;
		}
	} else {
		astScopeAddAnonIdent(scope, (SType *) &tempType);
	}

	return 1;
}

void mvSType(SType *dest, const SType *src) {
	memcpy(dest, src, sizeof(STypeBuf));
}

SType *movaSType(SType const *src) {
	SType *result = malloc(sizeof(STypeBuf));
	mvSType(result, src);
	return result;
}

#define X(NAME, STR) STR,
const char *_sttStr[] = {
	X_STYPE_TYPE
};
#undef X

const char *sttStr(STypeT t) {
	return _sttStr[t];
}

int printSType(SType const *type) {
	if (type->vtable->table.printFunc) {
		return type->vtable->table.printFunc(type);
	}
	return printf("\"unknown\"");
}

SType *stypeDeref(SType *type) {
	if (type->vtable->deref) {
		return type->vtable->deref(type);
	} else {
		return NULL;
	}
}

SType *stypeDerefMult(SType *type) {
	while (1) {
		SType *next = stypeDeref(type);
		if (!next) {
			return type;
		}
		type = next;
	}
}

static int _sprimCombine(SPrim *main, SPrim *next) {
	DEBUG_MSG("combining a sprin");
	return main->primType == next->primType;
}

static int _sarrayCombine(SArray *main, SArray *next) {
	if (!stypeCombine(main->elType, next->elType)) {
		return 0;
	}

	if (main->hasSize && next->hasSize) {
		return main->size == next->size;
	} else if (next->hasSize) {
		main->size = next->size;
	}
	return 1;
}

static int _scompoundCombine(SCompound *main, SCompound *next) {
	return 1;
}

static int _sfuncCombine(SFunction *main, SFunction *next) {
	return 1;
}

static int _spointerCombine(SPointer *main, SPointer *next) {
	return stypeCombine(main->internal, next->internal);
}

static int _senumCombine(SEnum *main, SEnum *next) {
	return 1;
}

int stypeCombine(SType *main, SType *next) {
	SType *lhs = stypeDerefMult(main);
	SType *rhs = stypeDerefMult(next);

	if (!lhs || !rhs) {
		INT_ERROR("invalid arg to stypeCombine");
		return 0;
	}

	if (lhs->type != rhs->type) {
		logCerr(CERR_TYPE, NULL, "Incompatible types");
		return 0;
	}

	if (!lhs->isExtern && !rhs->isExtern) {
		logCerr(CERR_TYPE, NULL, "Can't both be not extern");
		return 0;
	}

	lhs->isExtern = lhs->isExtern && rhs->isExtern;

	switch (lhs->type) {
		case STT_PRIM: return _sprimCombine((SPrim *) main, (SPrim *) next);
		case STT_ARRAY: return _sarrayCombine((SArray *) main, (SArray *) next);
		case STT_STRUCT:
		case STT_UNION: return _scompoundCombine((SCompound *) main, (SCompound *) next);
		case STT_FUNC: return _sfuncCombine((SFunction *) main, (SFunction *) next);
		case STT_POINTER: return _spointerCombine((SPointer *) main, (SPointer *) next);
		case STT_ENUM: return _senumCombine((SEnum *) main, (SEnum *) next);
		default: INT_ERROR("Trying to combine invalid type \"%s\"", sttStr(lhs->type));
	}
	DEBUG_MSG("about to exit");
	return 1;
}

void stypeLoadTypeQualifier(SType *type, ASTTypeQualifier qualifier) {
	if (qualifier & AST_TQ_CONST) {
		type->isConst = 1;
	}
	if (qualifier & AST_TQ_VOLATILE) {
		type->isVolatile = 1;
	}
}

void stypeLoadStorageClass(SType *type, ASTStorageClassSpec spec) {
	if (spec & AST_SC_TYPEDEF) {
		type->isTypedef = 1;
	}
	if (spec & AST_SC_REGISTER) {
		type->isRegister = 1;
	}
	if (spec & AST_SC_STATIC) {
		type->isStatic = 1;
	}
	if (spec & AST_SC_EXTERN) {
		type->isExtern = 1;
	}
}

SType *stypeGetIntern(SType *type) {
	if (type->vtable->internal) {
		return type->vtable->internal(type);
	}
	return NULL;
}
