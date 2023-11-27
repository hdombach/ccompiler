#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "../util/log.h"
#include "scope.h"
#include "../ast/declaration.h"

/*************************************************************
 * Semantic Type
 *************************************************************/

void initSType(SType *type) {
	type->type = STT_UNKNOWN;
	type->isConst = 0;
	type->isVolatile = 0;
	type->isTypedef = 0;
}

void destroySType(SType *type) {
	switch (type->type) {
		case STT_VOID: break;
		case STT_PRIM: break;
		case STT_ARRAY: destroySArray((SArray *) type); break;
		case STT_STRUCT: break;
		case STT_STRUCT_REF: break;
		case STT_UNION: break;
		case STT_UNION_REF: break;
		case STT_FUNC: destroySFunction((SFunction *) type); break;
		case STT_POINTER: destroySPointer((SPointer *) type); break;
		case STT_TYPEDEF_REF: break;
		case STT_ENUM: break;
		case STT_ENUM_REF: break;
		case STT_ENUM_CONST: break;
		case STT_UNKNOWN: break;
	}
	type->type = STT_UNKNOWN;
}

static int loadDecl(
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
	logDebug(
			"DEBUG DECL", "loadDecl %s (%s)",
			astNodeTypeStr(node->type),
			astDeclaratorName(node));
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
	if (!LOG_ASSERT(spec->node.type == AST_TYPE_SPEC)) return 0;

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
	if (!LOG_ASSERT(declaration->node.type == AST_DECLARATION)) return 0;

	STypeBuf tempInternal, tempType;

	if (!loadTypespec((SType *) &tempInternal, declaration->typeSpec, scope)) {
		logErrTok(declaration->node.tok, "Problem loading typespecs");
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
			logErrTok(
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
			logErrTok(
				declaration->node.tok,
				"Couldn't add identifier %s",
				astDeclaratorName((ASTNode *) declarator));
			return 0;
		}
	}

	return 1;
}

int loadParamSType(ASTScope *scope, struct ASTParam *param) {
	if (!LOG_ASSERT(param->node.type == AST_PARAM)) return 0;

	STypeBuf tempInternal, tempType;

	if (!loadTypespec((SType *) &tempInternal, param->typeSpec, scope)) {
		logErrTok(param->node.tok, "Problem loading param typespec");
		return 0;
	}

	if (!loadDecl(
			(SType *) &tempType,
			(SType *) &tempInternal,
			(ASTNode *) param->declarator,
			scope))
	{
		logErrTok(
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
			logErrTok(
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

const char *sttStr(STypeT t) {
	switch(t) {
		case STT_VOID: return "void";
		case STT_PRIM: return "primitive";
		case STT_ARRAY: return "array";
		case STT_STRUCT: return "structure";
		case STT_STRUCT_REF: return "structure ref";
		case STT_UNION: return "union";
		case STT_UNION_REF: return "union ref";
		case STT_FUNC: return "function";
		case STT_POINTER: return "pointer";
		case STT_TYPEDEF_REF: return "typedef ref";
		case STT_ENUM: return "enum";
		case STT_ENUM_REF: return "enum ref";
		case STT_ENUM_CONST: return "enumerator constant";
		default: return "(unknown)";
	}
}

int printSType(SType const *type) {
	switch (type->type) {
		case STT_PRIM: return printSPrim((SPrim *) type);
		case STT_ARRAY: return printSArray((SArray *) type);
		case STT_STRUCT:
		case STT_UNION: return printSCompound((SCompound *) type);
		case STT_STRUCT_REF:
		case STT_UNION_REF: return printSCompoundRef((SCompoundRef *) type);
		case STT_FUNC: return printSFunction((SFunction *) type);
		case STT_POINTER: return printSPointer((SPointer *) type);
		case STT_TYPEDEF_REF: return printSTypeRef((STypeRef *) type);
		case STT_ENUM_CONST: return printSPrim((SPrim *) type);
		default: return printf("{\"type\": \"%s\"}", sttStr(type->type)); 
	}
}

/*************************************************************
 * Semantic Primitive Type
 *************************************************************/

void initSPrim(SPrim *type) {
		initSType((SType *) type);
		type->primType = SPT_UNKNOWN;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

int loadSPrim(SPrim *type, ASTTypeSpec *typeSpec) {
	STYPE_VALID(SPrim);
	if (!LOG_ASSERT(typeSpec->node.type == AST_TYPE_SPEC)) return 0;

	ASTArithType arith = astArithTypeNormalize(typeSpec->arith);

	initSPrim(type);
	switch (arith) {
		case AST_AT_CHAR:
			type->primType = SPT_CHAR;
			break;
		case (AST_AT_SIGNED | AST_AT_CHAR):
			type->primType = SPT_SCHAR;
			break;
		case (AST_AT_UNSIGNED | AST_AT_CHAR):
			type->primType = SPT_UCHAR;
			break;
		case (AST_AT_SHORT | AST_AT_INT):
			type->primType = SPT_SHORT;
			break;
		case (AST_AT_UNSIGNED | AST_AT_SHORT | AST_AT_INT):
			type->primType = SPT_USHORT;
			break;
		case (AST_AT_INT):
			type->primType = SPT_INT;
			break;
		case (AST_AT_UNSIGNED | AST_AT_INT):
			type->primType = SPT_UINT;
			break;
		case (AST_AT_LONG | AST_AT_INT):
			type->primType = SPT_LONG;
			break;
		case (AST_AT_UNSIGNED | AST_AT_LONG | AST_AT_INT):
			type->primType = SPT_ULONG;
			break;
		case (AST_AT_FLOAT):
			type->primType = SPT_FLOAT;
			break;
		case (AST_AT_DOUBLE):
			type->primType = SPT_DOUBLE;
			break;
		case (AST_AT_LONG | AST_AT_DOUBLE):
			type->primType = SPT_LDOUBLE;
			break;
		default:
			return 0;
	}
	if (typeSpec->storage & AST_TST_TYPEDEF) {
		type->type.isTypedef = 1;
	}
	//lovely line of code here
	type->type.type = STT_PRIM;
	return 1;
}

#pragma clang diagnostic pop

const char *sptStr(SPrimType t) {
	switch (t) {
		case SPT_CHAR: return "char";
		case SPT_UCHAR: return "unsigned char";
		case SPT_SCHAR: return "signed char";
		case SPT_SHORT: return "short";
		case SPT_USHORT: return "unsigned short";
		case SPT_INT: return "int";
		case SPT_UINT: return "unsigned int";
		case SPT_LONG: return "long";
		case SPT_ULONG: return "unsigned long";
		case SPT_FLOAT: return "float";
		case SPT_DOUBLE: return "double";
		case SPT_LDOUBLE: return "long double";
		default: return "(unknown)";
	}
}

int printSPrim(const SPrim *type) {
	if (!LOG_ASSERT(type->type.type == STT_PRIM || type->type.type == STT_ENUM_CONST)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	n += printf(", \"content\": \"%s\"", sptStr(type->primType));

	n += printf("}");

	return n;
}
/*************************************************************
 * Semantic Array Type
 *************************************************************/

void initSArray(SArray *type) {
	initSType((SType *) type);

	type->elType = NULL;
	type->size = -1;
	type->hasSize = 0;
}

void destroySArray(SArray *type) {
	if (type->elType) {
		destroySType(type->elType);
		free(type->elType);
	}
}

int loadSArray(SArray *type, SType *internal, ASTArrayDecl *arrayDecl, ASTScope *scope) {
	STYPE_VALID(SArray);
	if (!LOG_ASSERT(arrayDecl->node.type == AST_ARRAY_DECL)) return 0;

	STypeBuf tempBuf;
	initSArray(type);

	if (arrayDecl->node.type != AST_ARRAY_DECL) {
		return 0;
	}

	if (!loadDecl((SType *) &tempBuf, internal, arrayDecl->encl, scope)) {
		return 0;
	}

	type->elType = movaSType((SType *) &tempBuf);
	type->type.type = STT_ARRAY;
	type->size = 0;
	TODO("Set size");
	return 1;
}

int printSArray(const SArray *type) {
	if (!LOG_ASSERT(type->type.type == STT_ARRAY)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	if (type->hasSize) {
		n += printf(", \"size\": %zu", type->size);
	}

	if (type->elType) {
		n += printf(", \"element type\": ");
		n += printSType(type->elType);
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Semantic Compound Types
 * Used for both unions and structs
 *************************************************************/

void initSCompound(SCompound *type) {
	initSType((SType *) type);
	type->scope = NULL;
	type->isUnion = 0;
}

int printSCompound(const SCompound *ref) {
	if (!LOG_ASSERT(ref->type.type == STT_STRUCT || ref->type.type == STT_UNION))
		return 0;
	return printASTScope(ref->scope);
}

int loadSCompound(SCompound *type, ASTStructDecl *decl) {
	if (!LOG_ASSERT(decl->node.type == AST_STRUCT_DECL)) return 0;
	type->isUnion = decl->isUnion;
	type->scope = decl->scope;
	if (type->isUnion) {
		type->type.type = STT_UNION;
	} else {
		type->type.type = STT_STRUCT;
	}
	return 1;
}

void initSCompoundRef(SCompoundRef *type) {
	initSType((SType *) type);
	type->index = -1;
	type->parentScope = NULL;
}

int loadSCompoundRef(
		SCompoundRef *type,
		ASTStructDecl *structDecl,
		ASTScope *scope)
{
	STYPE_VALID(SCompoundRef);
	if (!LOG_ASSERT(structDecl->node.type == AST_STRUCT_DECL)) return 0;

	SCompound tempComp;

	initSCompoundRef(type);
	if (structDecl->name) {
		if (!astScopeHasCompound(scope, structDecl->name)) {
			if (!loadSCompound(&tempComp, structDecl)) return 0;
			if (!astScopeAddCompound(scope, &tempComp, strdup(structDecl->name))) return 0;
		}
		*type = astScopeGetCompound(scope, structDecl->name);
		return 1;
	}

	if (!loadSCompound(&tempComp, structDecl)) return 0;
	*type = astScopeAddAnonCompound(scope, &tempComp);
	return 1;
}

int printSCompoundRef(const SCompoundRef *ref) {
	if (!LOG_ASSERT(ref->type.type == STT_STRUCT_REF || ref->type.type == STT_UNION_REF))
		return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(ref->type.type));

	n += printf(", \"content\": ");
	n += printSCompound(scompoundDeref((SCompoundRef*) ref));

	n += printf("}");

	return n;
}

SCompound *scompoundDeref(struct SCompoundRef *ref) {
	return dlistGetm(&ref->parentScope->tags, ref->index);
}

/*************************************************************
 * Semantic Pointer Type
 *************************************************************/

SPointer *newSPointer() {
	SPointer *result = malloc(STYPE_S);
	initSPointer(result);
	return result;
}

void initSPointer(SPointer *type) {
	initSType((SType *) type);
	type->internal = NULL;
}

void destroySPointer(SPointer *type) {
	if (type->internal) {
		destroySType(type->internal);
		free(type->internal);
	}
}

int loadSPointer(
		SType *type,
		SType *internal,
		ASTDeclarator *declarator,
		ASTScope *scope)
{
	STypeBuf pointer;
	pointer.type.type = STT_POINTER;
	((SPointer *) &pointer)->internal = movaSType(internal);

	if (!loadDecl(type, (SType *) &pointer, declarator->encl, scope)) goto failure;
	return 1;

failure:
	destroySPointer((SPointer *) &pointer);
	return 0;
}

int printSPointer(const SPointer *type) {
	if (!LOG_ASSERT(type->type.type == STT_POINTER)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	if (type->internal) {
		n += printf(", \"content\": ");
		n += printSType(type->internal);
	}

	n += printf("}");
	
	return n;
}

/*************************************************************
 * Semantic Function Type
 *************************************************************/

void initSFunction(SFunction *type) {
	initSType((SType *) type);
	type->returnType = NULL;
	type->paramScope = NULL;
}

void destroySFunction(SFunction *type) {
	if (type->returnType) {
		destroySType(type->returnType);
		free(type->returnType);
	}

	type->paramScope = NULL; //owned by the AST
}

int loadSFunction(
		SType *type,
		SType *internal,
		ASTFuncDecl *declarator,
		ASTScope *scope)
{
	if (!LOG_ASSERT(declarator->node.type == AST_FUNC_DECL)) return 0;
	STypeBuf func;
	SFunction *funcPtr = (SFunction *) &func;
	initSFunction(funcPtr);
	funcPtr->returnType = movaSType(internal);
	funcPtr->paramScope = declarator->scope;
	funcPtr->type.type = STT_FUNC;

	if (!loadDecl((SType *) type, (SType *) funcPtr, declarator->encl, scope)) return 0;
	return 1;
}

int printSFunction(const SFunction *func) {
	if (!LOG_ASSERT(func->type.type == STT_FUNC)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(func->type.type));

	if (func->returnType) {
		n += printf(", \"return type\": ");
		n += printSType(func->returnType);
	}

	n += printf(", \"params\": ");
	n += printASTScope(func->paramScope);

	n += printf("}");

	return n;
}

void initSTypeRef(STypeRef *type) {
	initSType((SType *) type);
	type->index = -1;
	type->parentScope = NULL;
}

int loadSTypeRef(STypeRef *type, ASTIdentifier *identifier, ASTScope *scope) {
	STYPE_VALID(STypeRef);

	if (!LOG_ASSERT(identifier->node.type == AST_IDENTIFIER_TS)) return 0;

	if (!astScopeGetIdentifier(type, scope, identifier->name)) return 0;

	type->type.type = STT_TYPEDEF_REF;
	return 1;
}

SType *stypdefDeref(STypeRef *ref) {
	return dlistGetm(&ref->parentScope->identifiers, ref->index);
}

int printSTypeRef(const STypeRef *type) {
	if (!LOG_ASSERT(type->type.type == STT_TYPEDEF_REF)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	n += printf(", \"content\": ");
	n += printSType(stypdefDeref((STypeRef *) type));

	n += printf("}");

	return n;
}

void initSEnum(SEnum *type) {
	initSType((SType *) type);
	type->scope = NULL;
}

int printSEnum(SEnum *type) {
	if (!LOG_ASSERT(type->type.type == STT_ENUM)) return 0;
	if (type->scope) {
		return printASTScope(type->scope);
	} else {
		return 0;
	}
}

int loadSEnum(SEnum *type, ASTEnumDecl *decl) {
	if (!LOG_ASSERT(decl->node.type == AST_ENUM_DECL)) return 0;
	STYPE_VALID(SEnum);
	type->type.type = STT_ENUM;
	return 1;
}

void initSEnumRef(SEnumRef *type) {
	initSType((SType *) type);
	type->index = -1;
	type->parentScope = NULL;
}

int loadSEnumRef(SEnumRef *type, ASTEnumDecl *declaration, ASTScope *scope) {
	STYPE_VALID(SEnumRef);
	if (!LOG_ASSERT(declaration->node.type == AST_ENUM_DECL)) return 0;

	SEnum tempEnum;

	initSEnumRef(type);
	if (declaration->name) {
		if (!astScopeHasEnum(scope, declaration->name)) {
			if (!loadSEnum(&tempEnum, declaration)) return 0;
			if (!astScopeAddEnum(scope, &tempEnum, strdup(declaration->name))) return 0;
		}
		*type = astScopeGetEnum(scope, declaration->name);
		return 1;
	}

	if (!loadSEnum(&tempEnum, declaration)) return 0;
	*type = astScopeAddAnonEnum(scope, &tempEnum);
	return 1;
}

int printSEnumRef(const SEnumRef *ref) {
	if (!LOG_ASSERT(ref->type.type == STT_ENUM_REF)) return 0;
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(ref->type.type));

	n += printASTScope(senumDeref(ref)->scope);

	n += printf("}");

	return n;
}

SEnum *senumDeref(SEnumRef const *ref) {
	return dlistGetm(&ref->parentScope->tags, ref->index);
}

int loadSEnumConst(SPrim *type, ASTEnumConst *decl, ASTScope *scope) {
	STYPE_VALID(SPrim);
	if (!LOG_ASSERT(decl->node.type == AST_ENUM_CONST)) return 0;

	type->type.type = STT_ENUM_CONST;
	type->primType = SPT_INT;

	return 1;
}
