#include <stdio.h>
#include <string.h>

#include "../ast/astUtil.h"
#include "scope.h"
#include "../ast/statement.h"

void initASTScope(ASTScope *scope) {
	initWordDict(&scope->typedefNames);
	scope->parent = NULL;
	initWordDict(&scope->labelDict);
	initDList(&scope->labels, sizeof(ASTStm *));
}

void freeASTScope(ASTScope *scope) {
	freeWordDict(&scope->typedefNames);
}

int astScopeIsTypedef(ASTScope *scope, const char *name) {
	if (wordDictPresent(&scope->typedefNames, name)) {
		return 1;
	} else if (scope->parent) {
		return astScopeIsTypedef(scope->parent, name);
	} else {
		return 0;
	}
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

ASTScopeErr astScopeAddLabel(ASTScope *scope, struct ASTStm *stm) {
	if (!stm->label || stm->label->type != AST_LBL_IDENTIFIER) {
		return SCOPE_INVALID_ARG;
	}

	ASTLblIdentifier *lbl = (ASTLblIdentifier *) stm->label;
	if (astScopeGetLabel(scope, lbl->name)) {
		return SCOPE_EXISTS;
	}

	int index = scope->labels.size;
	dlistApp(&scope->labels, stm);
	wordDictInsert(&scope->labelDict, strdup(lbl->name), index);
	return SCOPE_SUCCESS;
}

struct ASTStm *astScopeGetLabel(ASTScope *scope, const char *labelName) {
	int const *index = wordDictGet(&scope->labelDict, labelName);
	if (!index) {
		if (scope->parent) {
			return astScopeGetLabel(scope->parent, labelName);
		} else {
			return NULL;
		}
	}
	return dlistGetm(&scope->labels, *index);
}
