#include <stdio.h>

#include "astUtil.h"
#include "scope.h"
#include "../util/typeDict.h"
#include "type.h"

void initASTScope(ASTScope *scope) {
	initTypeDict(&scope->types);
	scope->parent = NULL;
}

void freeASTScope(ASTScope *scope) {
	freeTypeDict(&scope->types);
}

int printASTScope(ASTScope const *scope) {
	return printTypeDict(&scope->types);
}

int astScopeInsert(ASTScope *scope, ASTType type) {
	if (typeDictPresent(&scope->types, type.name)) {
		ASTType *old = astScopeGetmLoc(scope, type.name);
		ASTType *res = astTypeComp(old, &type);
		if (!res) {
			snprintf(astErrMsgBuf, AST_ERR_MSG_S, "Duplicate symbol %s", type.name);
			astErr(astErrMsgBuf, type.tok);
			freeASTType(&type);
			return 0;
		}
		if (old == res) {
			freeASTType(&type);
		} else {
			freeASTType(old);
			*old = type;
		}
		return 1;
	} else {
		return typeDictInsert(&scope->types, type);
	}
}

int astScopeInsertMult(ASTScope *scope, DList *types) {
	int result = 1;
	for (int i = 0; i < types->size; i++) {
		ASTType *type = (ASTType *) dlistGetm(types, i);
		result = astScopeInsert(scope, *type) && result;
	}
	free(types->data);
	return result;
}

int astScopePresent(const ASTScope *scope, const char *name) {
	int result = typeDictPresent(&scope->types, name);
	if (result) {
		return result;
	} else if (scope->parent) {
		return astScopePresent(scope->parent, name);
	} else {
		return result;
	}
}

ASTType const *astScopeGet(ASTScope const *scope, char const *name) {
	ASTType const *result = typeDictGet(&scope->types, name);
	if (result) {
		return result;
	} else if (scope->parent) {
		return astScopeGet(scope->parent, name);
	} else {
		return NULL;
	}
}

ASTType *astScopeGetm(ASTScope *scope, char const *name) {
	ASTType *result = typeDictGetm(&scope->types, name);
	if (result) {
		return result;
	} else if (scope->parent) {
		return astScopeGetm(scope->parent, name);
	} else {
		return NULL;
	}
}

ASTType *astScopeGetmLoc(ASTScope *scope, char *name) {
	return typeDictGetm(&scope->types, name);
}
