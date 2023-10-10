#include "compStatement.h"
#include "../util/dlist.h"
#include "declaration.h"
#include "statement.h"
#include "astUtil.h"
#include "../util/callbacks.h"

void initASTCompItem(ASTCompItem *item) {
	item->type = AST_CIT_UNKNOWN;
}

void freeASTCompItem(ASTCompItem *item) {
	switch (item->type) {
		case AST_CIT_STM:
			freeASTStm(&item->c.statement);
			break;
		case AST_CIT_DECL:
			freeASTDeclaration((ASTDeclaration *) &item->c.declaration);
			break;
		default:
			break;
	}
}

int parseASTCompItem(
		ASTCompItem *item,
		const Token *tok,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTStm stm;

	initASTCompItem(item);
	if (astHasErr()) {
		freeASTCompItem(item);
		return 0;
	}

	if ((res = parseASTStm(&item->c.statement, tok + n, scope))) {
		n += res;
		item->type = AST_CIT_STM;
	} else if ((res = parseASTDeclaration((ASTDeclaration *) &item->c.declaration, tok + n, scope))) {
		n += res;
		item->type = AST_CIT_DECL;
	} else {
		freeASTCompItem(item);
		return 0;
	}

	return n;
}

int printASTCompItem(const ASTCompItem *node) {
	int n = 0;

	switch (node->type) {
		case AST_CIT_STM:
			n += printASTStm(&node->c.statement);
			break;
		case AST_CIT_DECL:
			n += printASTDeclaration((ASTDeclaration *) &node->c.declaration);
			break;
		default:
			break;
	}

	return n;
}

void initASTCompStm(ASTCompStm *node) {
	initDListEmpty(&node->items, sizeof(ASTCompItem));
}

void freeASTCompStm(ASTCompStm *node) {
	freeDList(&node->items, (FreeFunc) freeASTCompItem);
}

int parseASTCompStm(
		ASTCompStm *node,
		const Token *tok,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTCompItem item;

	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type != TT_O_CURLY) {
		return 0;
	}
	n++;

	initASTCompStm(node);
	while (!astHasErr()) {
		if ((res = parseASTCompItem(&item, tok + n, scope))) {
			dlistApp(&node->items, &item);
			n += res;
		} else {
			break;
		}
	}

	if (tok[n].type != TT_C_CURLY) {
		freeASTCompStm(node);
		return 0;
	}
	n++;


	return n;
}

int printASTCompStm(const ASTCompStm *node) {
	int n = 0;

	n += printDList(&node->items, (PrintFunc) printASTCompItem);

	return n;
}
