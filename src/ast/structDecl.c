#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "structDecl.h"
#include "astUtil.h"
#include "declaration.h"
#include "scope.h"
#include "../util/callbacks.h"

void initASTStructDecl(ASTStructDecl *decl) {
	initASTNode((ASTNode *) decl);
	decl->name = NULL;
	initDList(&decl->items, AST_NODE_S);
	decl->scope = malloc(sizeof(ASTScope));
	initASTScope(decl->scope);
	decl->isUnion = 0;
}

void freeASTStructDecl(ASTStructDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	freeDList(&decl->items, (FreeFunc) freeASTNode);
	freeASTScope(decl->scope);
	free(decl->scope);
}

int parseASTStructDecl(
		ASTStructDecl *decl,
		const Token *tok,
		ASTScope const *scope)
{
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}

	initASTStructDecl(decl);
	decl->scope->parent = (ASTScope *) scope; //is const in this context

	if (tok[n].type == TT_STRUCT) {
		n++;
	} else if (tok[n].type == TT_UNION) {
		decl->isUnion = 1;
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
		return n;
	}

	while (1) {
		ASTNodeBuf tempBuf;
		if ((res = parseASTDeclaration((ASTDeclaration *) &tempBuf, tok + n, decl->scope))) {
			n += res;
			dlistApp(&decl->items, &tempBuf);
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		astErr("Expecting }", tok + n);
		freeASTStructDecl(decl);
		return 0;
	}

	//decl->node.type = AST_STRU

	return n;
}

int printASTStructDecl(const ASTStructDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"struct decl\"");

	if (decl->name) {
		if (decl->isUnion) {
			n += printf(", \"union name\": ");
		} else {
			n += printf(", \"struct name\": ");
		}
		n += printJsonStr(decl->name);
	}

	if (decl->isUnion) {
		n += printf(", \"union declarations\": ");
	} else {
		n += printf(", \"struct declarations\": ");
	}
	n += printDList(&decl->items, (PrintFunc) printASTDeclaration);

	n += printf("}");

	return n;
}
