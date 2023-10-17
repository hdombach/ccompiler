#include "file.h"
#include "declaration.h"
#include "funcDef.h"
#include "scope.h"
#include "astUtil.h"
#include "../util/callbacks.h"
#include "node.h"
#include <stdio.h>

int parseASTFileItem(
		ASTNode *item,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;

	initASTNode(item);
	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTFuncDef((ASTFuncDef *) item, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTDeclaration((ASTDeclaration *) item, tok + n, scope))) {
		n += res;
	} else {
		return 0;
	}

	return n;
}

DList astFileItemTypes(const ASTNode *item) {
	DList result;

	switch (item->type) {
		case AST_DECLARATION:
			result = astDeclarationTypedefNames((ASTDeclaration *) item);
			break;
		default:
			initDListEmpty(&result, sizeof(char *));
			break;
	}

	return result;
}

void initASTFile(ASTFile *file) {
	initDList(&file->items, AST_NODE_S);
	initASTScope(&file->scope);
}

void freeASTFile(ASTFile *file) {
	freeDList(&file->items, (FreeFunc) freeASTNode);
	freeASTScope(&file->scope);
}

int parseASTFile(ASTFile *file, const Token *tok) {
	AST_VALID(ASTFile);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFile(file);

	while (1) {
		if ((res = parseASTFileItem((ASTNode *) &tempBuf, tok + n, &file->scope))) {
			n += res;
			dlistApp(&file->items, &tempBuf);

			DList newTypes = astFileItemTypes((ASTNode *) &tempBuf);
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
	n += printDList(&file->items, (PrintFunc) printASTNode);

	n += printf("}");

	return n;
}
