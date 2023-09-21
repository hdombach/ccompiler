#include "file.h"
#include "declaration.h"
#include "funcDef.h"
#include "scope.h"
#include "astUtil.h"
#include "../util/callbacks.h"
#include <stdio.h>

void initASTFileItem(ASTFileItem *item) {
	item->type = AST_FIT_UNKNOWN;
}

void freeASTFileItem(ASTFileItem *item) {
	switch (item->type) {
		case AST_FIT_DECL:
			freeASTDeclaration(&item->c.declaration);
			break;
		case AST_FIT_FUNC_DEF:
			freeASTFuncDef(&item->c.funcDef);
			break;
		default:
			break;
	}
}

int parseASTFileItem(
		ASTFileItem *item,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;

	initASTFileItem(item);
	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTFuncDef(&item->c.funcDef, tok + n, scope))) {
		n += res;
		item->type = AST_FIT_FUNC_DEF;
	} else if ((res = parseASTDeclaration(&item->c.declaration, tok + n, scope))) {
		n += res;
		item->type = AST_FIT_DECL;
	} else {
		freeASTFileItem(item);
		return 0;
	}

	return n;
}

int printASTFileItem(const ASTFileItem *item) {
	int n = 0;

	switch (item->type) {
		case AST_FIT_DECL:
			n += printASTDeclaration(&item->c.declaration);
			break;
		case AST_FIT_FUNC_DEF:
			n += printASTFuncDef(&item->c.funcDef);
			break;
		default:
			break;
	}
	return n;
}

DList astFileItemTypes(const ASTFileItem *item) {
	DList result;

	switch (item->type) {
		case AST_FIT_DECL:
			result = astDeclarationTypedefNames(&item->c.declaration);
			break;
		default:
			initDListEmpty(&result, sizeof(char *));
			break;
	}

	return result;
}

void initASTFile(ASTFile *file) {
	initDList(&file->items, sizeof(ASTFileItem));
	initASTScope(&file->scope);
}

void freeASTFile(ASTFile *file) {
	freeDList(&file->items, (FreeFunc) freeASTFileItem);
	freeASTScope(&file->scope);
}

int parseASTFile(ASTFile *file, const Token *tok) {
	int n = 0, res;
	ASTFileItem tempItem;

	initASTFile(file);

	while (1) {
		if ((res = parseASTFileItem(&tempItem, tok + n, &file->scope))) {
			n += res;
			dlistApp(&file->items, &tempItem);

			DList newTypes = astFileItemTypes(&tempItem);
			astScopeAddTypedefNames(&file->scope, newTypes);
		} else if (tok[n].type == TT_NEWLINE) {
			n++;
		} else {
			break;
		}
	}
	if (astHasErr()) {
		fprintASTErr(stderr);
	}

	return n;
}

int printASTFile(const ASTFile *file) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"file\"");

	n += printf(", \"declerations\": ");
	n += printDList(&file->items, (PrintFunc) printASTFileItem);

	n += printf("}");

	return n;
}
