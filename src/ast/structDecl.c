#include <string.h>

#include "structDecl.h"
#include "astUtil.h"
#include "declaration.h"
#include "scope.h"
#include "../util/callbacks.h"

void initASTStructDeclItem(ASTStructDeclItem *item) {
	item->type = AST_SDT_UNKNOWN;
}

void freeASTStructDeclItem(ASTStructDeclItem *item) {
	switch (item->type) {
		case AST_SDT_VAR:
			freeASTDeclaration(&item->c.declaration);
			break;
		default:
			break;
	}
}

int parseASTStructDeclItem(
		ASTStructDeclItem *item,
		const Token *tok,
		ASTScope *scope)
{
	int res, n = 0;

	initASTStructDeclItem(item);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTDeclaration(&item->c.declaration, tok + n, scope))) {
		item->type = AST_SDT_VAR;
		n += res;
	}

	return n;
}

int printASTStructDeclItem(const ASTStructDeclItem *item) {
	switch (item->type) {
		case AST_SDT_VAR:
			return printASTDeclaration(&item->c.declaration);
		default:
			return 0;
	}
}

void initASTStructDecl(ASTStructDecl *decl) {
	decl->name = NULL;
	initDList(&decl->items, sizeof(ASTStructDeclItem));
	initASTScope(&decl->scope);
}

void freeASTStructDecl(ASTStructDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	freeDList(&decl->items, (FreeFunc) freeASTStructDeclItem);
	freeASTScope(&decl->scope);
}

int parseASTStructDecl(ASTStructDecl *decl, const Token *tok, ASTScope *scope) {
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}

	initASTStructDecl(decl);
	decl->scope.parent = scope;

	if (tok[n].type == TT_STRUCT) {
		n++;
	} else {
		freeASTStructDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	}

	if (tok[n].type == TT_O_CURLY) {
		n++;
	} else {
		freeASTStructDecl(decl);
		return 0;
	}

	while (1) {
		ASTStructDeclItem item;
		if ((res = parseASTStructDeclItem(&item, tok + n, &decl->scope))) {
			n += res;
			dlistApp(&decl->items, &item);
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		freeASTStructDecl(decl);
		return 0;
	}

	return n;
}

int printASTStructDecl(const ASTStructDecl *decl) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (decl->name) {
		n += printf("\"struct name\": %s", decl->name);
		isFirst = 0;
	}

	if (!isFirst) {
		n += printf(", ");
	}
	n += printf("\"struct declarations\": ");
	n += printDList(&decl->items, (PrintFunc) printASTStructDeclItem);

	n += printf("}");

	return n;
}
