#include "file.h"
#include "declaration.h"
#include "scope.h"
#include "type.h"
#include "astUtil.h"
#include "../util/callbacks.h"

void initASTFileItem(ASTFileItem *item) {
	item->type = AST_FIT_UNKNOWN;
}

void freeASTFileItem(ASTFileItem *item) {
	switch (item->type) {
		case AST_FIT_DECL:
			freeASTDeclaration(&item->c.declaration);
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

	if ((res = parseASTDeclaration(&item->c.declaration, tok + n, scope))) {
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
		default:
			break;
	}
	return n;
}

DList astFileItemTypes(const ASTFileItem *item) {
	DList result;

	switch (item->type) {
		case AST_FIT_DECL:
			result = astDeclarationTypes(&item->c.declaration);
			break;
		default:
			initDListEmpty(&result, sizeof(ASTType));
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
			if (!astScopeInsertMult(&file->scope, &newTypes)) break;
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

	n += printf("\"scope\": ");
	n += printASTScope(&file->scope);

	n += printf(", \"declerations\": ");
	n += printDList(&file->items, (PrintFunc) printASTFileItem);

	n += printf("}");

	return n;
}
