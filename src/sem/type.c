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
#include "styperef.h"
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
			return loadSTypeRef(
				(STypeRef *) type,
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

		if (!astScopeAddIdent(
				scope,
				(SType *) &tempType,
				strdup(astDeclaratorName((ASTNode *) declarator))))
		{
			logCerr(
					CERR_UNKNOWN,
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
