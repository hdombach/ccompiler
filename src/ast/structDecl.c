#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "structDecl.h"
#include "astUtil.h"
#include "declaration.h"
#include "../sem/scope.h"
#include "../util/callbacks.h"

void initASTStructDecl(ASTStructDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	decl->name = NULL;
	initDListEmpty(&decl->items, AST_NODE_S);
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
	AST_VALID(ASTStructDecl);
	int res, n = 0;

	decl->node.type = AST_UNKNOWN;
	if (astHasErr()) {
		return 0;
	}

	initASTStructDecl(decl, tok);
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
		decl->node.type = AST_STRUCT_DECL;
		return n;
	}

	ASTNodeBuf tempBuf;
	while (1) {
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

	decl->node.type = AST_STRUCT_DECL;
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
	n += printDList(&decl->items, (PrintFunc) printASTNode);

	n += printf("}");

	return n;
}

ASTTravRes astStructDeclTrav(
		ASTStructDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	for (int i = 0; i < node->items.size; i++) {
		ASTNode *item = dlistGetm(&node->items, i);
		result = astNodeTrav(item, beforeFunc, afterFunc,
				ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}
