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
	initWordDict(&scope->tagDict);
	initDList(&scope->tags, sizeof(STypeBuf));
	initWordDict(&scope->identifierDict);
	initDList(&scope->identifiers, sizeof(STypeBuf));
}

void freeASTScope(ASTScope *scope) {
	freeWordDict(&scope->typedefNames);
	freeWordDict(&scope->labelDict);
	freeDList(&scope->labels, NULL);
	freeWordDict(&scope->tagDict);
	freeDList(&scope->tags, NULL);
	freeWordDict(&scope->identifierDict);
	freeDList(&scope->identifiers, (FreeFunc) destroySType);
}


typedef struct ScopePrintCxt {
	int isFirst;
	int n;
	DList *list;
	int *completed;
	const char **identifierNames;
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
	ctx->identifierNames[*value] = strdup(*key);
}

int printASTScope(ASTScope const *scope) {
	ScopePrintCxt ctx = {1, 0, NULL, NULL, NULL};
	if (!scope) return printf("\"(null)\"");

	ctx.n += printf("{");

	ctx.n += printf("\"type\": \"scope\"");

	ctx.n += printf(", \"identifier size\": %d", scope->identifiers.size);

	if (scope->labelDict.elementCount) {
		ctx.n += printf(", \"labels\": [");
		wordDictIter((WordDict *) &scope->labelDict, (WordDictIterFunc) _printScopeLabels, &ctx);
		ctx.n += printf("]");
	}

	if (scope->tags.size) {
		ctx.list = (DList *) &scope->tags;
		ctx.completed = calloc(1, sizeof(int) * scope->tags.size);

		wordDictIter((WordDict *) &scope->tagDict, (WordDictIterFunc) _printScopeStructs, &ctx);

		for (int i = 0; i < scope->tags.size; i++) {
			if (ctx.completed[i]) continue;
			ctx.n += printf(", \"anonymous-%d\": ", i);
			ctx.n += printSType(dlistGet(&scope->tags, i));
		}

		free(ctx.completed);
	}

	if (scope->identifiers.size) {
		ctx.identifierNames = calloc(scope->identifiers.size, sizeof(char *));
		ctx.list = (DList *) &scope->identifiers;
		ctx.completed = calloc(1, sizeof(int) * scope->identifiers.size);

		wordDictIter((WordDict *) &scope->identifierDict, (WordDictIterFunc) _printScopeIdentifiers, &ctx);

		//printf("identifer size is %d\n", scope->identifiers.size);
		for (int i = 0; i < scope->identifiers.size; i++) {
			if (ctx.identifierNames[i]) {
				ctx.n += printf(", \"%s\": ", ctx.identifierNames[i]);
				free((void *) ctx.identifierNames[i]);
			} else {
				ctx.n += printf(", \"anonymous-%d\": ", i);
			}
			ctx.n += printSType(dlistGet(&scope->identifiers, i));
		}
		free(ctx.identifierNames);
		free(ctx.completed);
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
	return wordDictPresent(&scope->tagDict, name);
}

int astScopeAddCompound(ASTScope *scope, struct SCompound *compound, char *name) {
	if (!wordDictInsert(&scope->tagDict, name, scope->tags.size)) return 0;
	dlistApp(&scope->tags, compound);
	return 1;
}

struct SCompoundRef astScopeGetCompound(ASTScope *scope, const char *name) {
	SCompoundRef result;
	result.index = *wordDictGet(&scope->tagDict, name);
	result.parentScope = scope;

	
	SCompound *comp = dlistGetm(&scope->tags, result.index);
	if (comp->isUnion) {
		result.type.type = STT_UNION_REF;
	} else {
		result.type.type = STT_STRUCT_REF;
	}
	return result;
}

SCompoundRef astScopeAddAnonCompound(ASTScope *scope, struct SCompound *new) {
	SCompoundRef result;
	result.index = scope->tags.size;
	result.parentScope = scope;
	dlistApp(&scope->tags, new);

	if (new->isUnion) {
		result.type.type = STT_UNION_REF;
	} else {
		result.type.type = STT_STRUCT_REF;
	}

	return result;
}

int astScopeAddIdent(ASTScope *scope, struct SType *type, char *name) {
	int index;
	index = scope->identifiers.size;

	dlistApp(&scope->identifiers, type);
	return wordDictInsert(&scope->identifierDict, name, index);
}

int astScopeAddAnonIdent(ASTScope *scope, struct SType *type) {
	int index;
	index = scope->identifiers.size;
	dlistApp(&scope->identifiers, type);
	return index;
}

int astScopeGetIdentifier(STypedefRef *ref, ASTScope *scope, char *name) {
	const int *index = wordDictGet(&scope->identifierDict, name);
	if (!index) return 0;
	ref->index = *index;
	ref->parentScope = scope;
	return 1;
}

int astScopeHasEnum(ASTScope *scope, const char *name) {
	return wordDictPresent(&scope->tagDict, name);
}

int astScopeAddEnum(ASTScope *scope, struct SEnum *type, char *name) {
	if (!wordDictInsert(&scope->tagDict, name, scope->tags.size)) return 0;
	dlistApp(&scope->tags, type);
	return 1;
}

struct SEnumRef astScopeGetEnum(ASTScope *scope, const char *name) {
	SEnumRef result;
	result.index = *wordDictGet(&scope->tagDict, name);
	result.parentScope = scope;
	result.type.type = STT_ENUM_REF;

	return result;
}

struct SEnumRef astScopeAddAnonEnum(ASTScope *scope, struct SEnum *new) {
	SEnumRef result;
	result.index = scope->tags.size;
	result.parentScope = scope;
	dlistApp(&scope->tags, new);
	result.type.type = STT_ENUM_REF;

	return result;
}
