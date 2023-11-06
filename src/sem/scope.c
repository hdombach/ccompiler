#include <stdio.h>
#include <string.h>

#include "../ast/astUtil.h"
#include "scope.h"
#include "../ast/statement.h"
#include "type.h"

void initASTScope(ASTScope *scope) {
	initWordDict(&scope->typedefNames);
	scope->parent = NULL;
	initWordDict(&scope->labelDict);
	initDList(&scope->labels, sizeof(ASTStm *));
	initWordDict(&scope->structDict);
	initDList(&scope->structs, sizeof(SCompound));
	initWordDict(&scope->identifierDict);
	initDList(&scope->identifiers, sizeof(STypeBuf));
}

void freeASTScope(ASTScope *scope) {
	freeWordDict(&scope->typedefNames);
	freeWordDict(&scope->labelDict);
	freeDList(&scope->labels, NULL);
	freeWordDict(&scope->structDict);
	freeDList(&scope->structs, NULL);
	freeWordDict(&scope->identifierDict);
	freeDList(&scope->identifiers, (FreeFunc) destroySType);
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

int astScopeHasCompound(ASTScope *scope, const char *name) {
	return wordDictPresent(&scope->structDict, name);
}

int astScopeAddCompound(ASTScope *scope, struct SCompound *compound, char *name) {
	if (!wordDictInsert(&scope->structDict, name, scope->structs.size)) return 0;
	dlistApp(&scope->structs, compound);
	return 1;
}

struct SCompoundRef astScopeGetCompound(ASTScope *scope, const char *name) {
	SCompoundRef result;
	result.index = *wordDictGet(&scope->structDict, name);
	result.parentScope = scope;
	return result;
}

SCompoundRef astScopeAddAnonCompound(ASTScope *scope, struct SCompound *new) {
	SCompoundRef result;
	result.index = scope->structs.size;
	result.parentScope = scope;
	dlistApp(&scope->structs, new);
	return result;
}

struct SCompoundRef astScopeAddIdentifier(ASTScope *scope, struct SType *type, char *name) {
	SCompoundRef result;
	result.parentScope = scope;
	result.index = scope->structs.size;

	dlistApp(&scope->structs, type);
	wordDictInsert(&scope->structDict, name, result.index);

	return result;
}
