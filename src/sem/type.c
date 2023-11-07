#include "type.h"
#include "../util/log.h"
#include "scope.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************************************
 * Semantic Type
 *************************************************************/

void initSType(SType *type) {
	type->type = STT_UNKNOWN;
}

void destroySType(SType *type) {
	switch (type->type) {
		case STT_VOID: break;
		case STT_PRIM: break;
		case STT_ARRAY: destroySArray((SArray *) type); break;
		case STT_STRUCT: break;
		case STT_UNION: break;
		case STT_FUNC: destroySFunction((SFunction *) type); break;
		case STT_POINTER: destroySPointer((SPointer *) type); break;
		case STT_UNKNOWN: break;
	}
	type->type = STT_UNKNOWN;
}

static int loadDecl(SType *type, SType *internal, ASTDeclarator *declarator) {
	switch (declarator->node.type) {
		case AST_POINTER_DECL: return loadSPointer((SPointer *) type, internal, declarator);
		case AST_ARRAY_DECL: return loadSArray((SArray *) type, internal, declarator);
		case AST_FUNC_DECL: return loadSFunction((SFunction *) type, internal, declarator);
		case AST_IDENTIFIER_DECL: 
				mvSType(type, internal);
				return 1;
		default: return 0;
	};
	return 0;
};

static int loadTypespec(SType *type, ASTTypeSpec *spec, ASTScope *scope) {
	if (!LOG_ASSERT(spec->node.type == AST_TYPE_SPEC)) return 0;

	switch (spec->typeSpecType) {
		case AST_TST_UNION:
		case AST_TST_STRUCT:
			return loadSCompoundRef((SCompoundRef *) type, (ASTStructDecl *) spec->content, scope);
		case AST_TST_ENUM:
			TODO("add enum type spec");
			return 0;
		case AST_TST_VOID:
			type->type = STT_VOID;
			return 1;
		case AST_TST_ARITH:
			return loadSPrim((SPrim *) type, spec);
		case AST_TST_TYPEDEF:
			TODO("Add typedef type spec");
			return 0;
		case AST_TST_UNKNOWN:
			return 0;
	}
}

int loadSTypes(ASTScope *scope, ASTDeclaration *declaration) {
	STypeBuf tempInternal, tempType;

	if (!loadTypespec((SType *) &tempInternal, declaration->typeSpec, scope)) return 0;

	for (int i = 0; i < declaration->declarators.size; i++) {
		ASTDeclarator *declarator = dlistGetm(&declaration->declarators, i);
		/*technically, loadDecl takes ownership of internal. However
		 * since typespec types are trivially copiable, tis fine */
		if (!loadDecl((SType *) &tempType, (SType *) &tempInternal, declarator)) return 0;

		astScopeAddIdentifier(scope, (SType *) &tempType, strdup(astDeclaratorName(declarator)));
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
		case STT_UNION: return "union";
		case STT_FUNC: return "function";
		case STT_POINTER: return "pointer";
		default: return "(unknown)";
	}
}

int printSType(SType const *type) {
	switch (type->type) {
		case STT_PRIM: return printSPrim((SPrim *) type);
		case STT_ARRAY: return printSArray((SArray *) type);
		case STT_STRUCT:
		case STT_UNION: return printSCompoundRef((SCompoundRef *) type);
		case STT_FUNC: return printSFunction((SFunction *) type);
		case STT_POINTER: return printSPointer((SPointer *) type);
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
	if (!LOG_ASSERT(typeSpec->node.type == AST_TYPE_SPEC)) return 0;
	initSPrim(type);
	switch (typeSpec->arith) {
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

int loadSArray(SArray *type, SType *internal, ASTDeclarator *declarator) {
	STypeBuf tempBuf;
	initSArray(type);

	if (declarator->node.type != AST_ARRAY_DECL) {
		return 0;
	}

	if (!loadDecl((SType *) &tempBuf, internal, (ASTDeclarator *) declarator->encl)) {
		return 0;
	}

	type->elType = movaSType((SType *) &tempBuf);
	type->type.type = STT_ARRAY;
	//TODO: set size
	return 1;
}

int printSArray(const SArray *type) {
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
	type->scope = NULL;
	type->isUnion = 0;
}

int loadSCompound(SCompound *type, ASTStructDecl *decl) {
	type->isUnion = decl->isUnion;
	type->scope = decl->scope;
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
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(ref->type.type));

	TODO("finish");

	n += printf("}");

	return n;
}

/*************************************************************
 * Semantic Pointer Type
 *************************************************************/

void initSPointer(SPointer *type) {
	initSType((SType *) type);
	type->internal = NULL;
}

void destroySPointer(SPointer *type) {
	destroySType(type->internal);
	free(type->internal);
}

int loadSPointer(SPointer *type, SType *internal, ASTDeclarator *declarator) {
	initSPointer(type);
	TODO("Impliment loadSPointer");
	return 0;
}

int printSPointer(const SPointer *type) {
	int n = 0;

	n += printf("}");

	n += printf("\"type\": \"%s\"", sttStr(type->type.type));

	if (type->internal) {
		n += printf(", \"content\": ");
		n += printSType(type->internal);
	}

	return n;
}

/*************************************************************
 * Semantic Function Type
 *************************************************************/

void initSFunction(SFunction *type) {
	initSType((SType *) type);
	type->returnType = NULL;
	initDListEmpty(&type->paramTypes, sizeof(STypeBuf));
}

void destroySFunction(SFunction *type) {
	if (type->returnType) {
		destroySType(type->returnType);
		free(type->returnType);
	}

	freeDList(&type->paramTypes, (FreeFunc) destroySType);
}

int loadSFunction(SFunction *type, SType *internal, ASTDeclarator *declarator) {
	initSFunction(type);
	TODO("Impliment loadSFunction");
	return 0;
}

int printSFunction(const SFunction *func) {
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"%s\"", sttStr(func->type.type));

	if (func->returnType) {
		n += printf(", \"return type\": ");
		n += printSType(func->returnType);
	}

	n += printDList(&func->paramTypes, (PrintFunc) printSType);

	n += printf("}");

	return n;
}