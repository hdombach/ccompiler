#include "file.h"
#include "declaration.h"

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

int parseASTFileItem(ASTFileItem *item, const Token *tok) {
	int n = 0, res;

	initASTFileItem(item);

	if ((res = parseASTDeclaration(&item->c.declaration, tok + n))) {
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

void initASTFile(ASTFile *file) {
	initDList(&file->items, sizeof(ASTFileItem));
}

void freeASTFile(ASTFile *file) {
	freeDList(&file->items, (DListFreeFunc) freeASTFileItem);
}

int parseASTFile(ASTFile *file, const Token *tok) {
	int n = 0, res;
	ASTFileItem tempItem;

	initASTFile(file);

	while (1) {
		if ((res = parseASTFileItem(&tempItem, tok + n))) {
			n += res;
			dlistApp(&file->items, &tempItem);
		} else {
			break;
		}
	}

	return n;
}

int printASTFile(const ASTFile *file) {
	int n = 0;

	n += printDList(&file->items, (DListPrintFunc) printASTFileItem);

	return n;
}
