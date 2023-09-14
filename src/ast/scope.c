#include <stdio.h>
#include <string.h>

#include "astUtil.h"
#include "scope.h"

void initASTScope(ASTScope *scope) {
	initWordDict(&scope->typedefNames);
	scope->parent = NULL;
}

void freeASTScope(ASTScope *scope) {
	freeWordDict(&scope->typedefNames);
}

int astScopeIsTypedef(const ASTScope *scope, const char *name) {
	return wordDictPresent(&scope->typedefNames, name);
}

void astScopeAddTypedefName(ASTScope *scope, char *name) {
	wordDictInsert(&scope->typedefNames, name, 0);
}

void astScopeAddTypedefNames(ASTScope *scope, DList names) {
	for (int i = 0; i < names.size; i++) {
		char *name = *(char **) dlistGet(&names, i);
		astScopeAddTypedefName(scope, name);
	}
	if (names.data) {
		free(names.data);
	}
}
