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
	return typeDictInsert(&scope->types, type);
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
