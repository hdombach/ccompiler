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


typedef struct ScopePrintCxt {
	int isFirst;
	int n;
	DList *list;
	int *completed;
} ScopePrintCxt;

void static _printScopeLabels(const char **key, int *value, ScopePrintCxt *ctx) {
	if (ctx->isFirst) {
		ctx->isFirst = 0;
	} else {
		ctx->n += printf(", ");
	}
	ctx->n += printf("\"%s\"", *key);
}

void static _printScopeStructs(const char **key, int *value, ScopePrintCxt *ctx) {
	ctx->completed[*value] = 1;
	ctx->n += printf(", \"%s\": ", *key);
	ctx->n += printSType((const SType *) dlistGetm(ctx->list, *value));
}

void static _printScopeIdentifiers(const char **key, int *value, ScopePrintCxt *ctx) {
	ctx->n += printf(", \"%s\": ", *key);
	ctx->n += printSType((const SType *) dlistGetm(ctx->list, *value));
}

int printASTScope(ASTScope const *scope) {
	ScopePrintCxt ctx = {1, 0, NULL};

	ctx.n += printf("{");

	ctx.n += printf("\"type\": \"scope\"");

	if (scope->labelDict.elementCount) {
		ctx.n += printf(", \"labels\": [");
		wordDictIter((WordDict *) &scope->labelDict, (WordDictIterFunc) _printScopeLabels, &ctx);
		ctx.n += printf("]");
	}

	if (scope->structs.size) {
		ctx.isFirst = 1;
		ctx.list = (DList *) &scope->structs;
		ctx.completed = calloc(1, sizeof(int) * scope->structs.size);

		wordDictIter((WordDict *) &scope->structDict, (WordDictIterFunc) _printScopeStructs, &ctx);

		for (int i = 0; i < scope->structs.size; i++) {
			if (ctx.completed[i]) continue;
			ctx.n += printf(", \"anonymous\": ");
			ctx.n += printSType(dlistGet(&scope->structs, i));
		}

		free(ctx.completed);
	}

	if (scope->identifiers.size) {
		ctx.list = (DList *) &scope->identifiers;
		wordDictIter((WordDict *) &scope->identifierDict, (WordDictIterFunc) _printScopeIdentifiers, &ctx);
	}

	ctx.n += printf("}");

	return ctx.n;
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

	
	SCompound *comp = dlistGetm(&scope->structs, result.index);
	if (comp->isUnion) {
		result.type.type = STT_UNION;
	} else {
		result.type.type = STT_STRUCT;
	}
	return result;
}

SCompoundRef astScopeAddAnonCompound(ASTScope *scope, struct SCompound *new) {
	SCompoundRef result;
	result.index = scope->structs.size;
	result.parentScope = scope;
	dlistApp(&scope->structs, new);

	if (new->isUnion) {
		result.type.type = STT_UNION;
	} else {
		result.type.type = STT_STRUCT;
	}

	return result;
}

int astScopeAddIdentifier(ASTScope *scope, struct SType *type, char *name) {
	int index;
	index = scope->identifiers.size;

	dlistApp(&scope->identifiers, type);
	return wordDictInsert(&scope->identifierDict, name, index);
}
