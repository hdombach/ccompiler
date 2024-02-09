#include "senum.h"

#include <stdio.h>
#include <string.h>

#include "type.h"
#include "sprim.h"
#include "../util/log.h"

static STypeVTable _enumVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printSEnum,
	}
};

void initSEnum(SEnum *type) {
	initSType((SType *) type, &_enumVTable);
	type->scope = NULL;
}

int printSEnum(SEnum *type) {
	if (!ASSERT(type->type.type == STT_ENUM)) return 0;
	if (type->scope) {
		return printASTScope(type->scope);
	} else {
		return 0;
	}
}

int loadSEnum(SEnum *type, ASTEnumDecl *decl) {
	if (!ASSERT(decl->node.type == AST_ENUM_DECL)) return 0;
	STYPE_VALID(SEnum);
	type->type.type = STT_ENUM;
	return 1;
}

void initSEnumRef(SEnumRef *type) {
	type->index = -1;
	type->parentScope = NULL;
}

int loadSEnumRef(SEnumRef *type, ASTEnumDecl *declaration, ASTScope *scope) {
	STYPE_VALID(SEnumRef);
	if (!ASSERT(declaration->node.type == AST_ENUM_DECL)) return 0;

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
	int n = 0;

	n += printf("{");

	n += printf("\"type\": \"Enum Ref\"");

	n += printASTScope(senumDeref(ref)->scope);

	n += printf("}");

	return n;
}

SEnum *senumDeref(SEnumRef const *ref) {
	return dlistGetm(&ref->parentScope->tags, ref->index);
}

int loadSEnumConst(SPrim *type, ASTEnumConst *decl, ASTScope *scope) {
	STYPE_VALID(SPrim);
	if (!ASSERT(decl->node.type == AST_ENUM_CONST)) return 0;

	type->type.type = STT_ENUM_CONST;
	type->primType = SPT_INT;

	return 1;
}
