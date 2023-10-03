#include <stdlib.h>

#include "arrayDecl.h"
#include "declaration.h"
#include "expression.h"
#include "astUtil.h"
#include "node.h"
#include "structDecl.h"

void initASTArrayDecl(ASTArrayDecl *decl) {
	decl->exp = NULL;
	decl->encl = NULL;
}

void freeASTArrayDecl(ASTArrayDecl *decl) {
	if (decl->exp) {
		freeASTNode(decl->exp);
		free(decl->exp);
	}
	if (decl->encl) {
		freeASTDeclarator(decl->encl);
		free(decl->encl);
	}
}

int parseASTArrayDecl(
		ASTArrayDecl *decl,
		const Token *tok,
		ASTDeclarator *encl,
		ASTScope const *scope) {
	int n = 0, res;

	initASTArrayDecl(decl);
	if (encl) {
		decl->encl = malloc(sizeof(ASTDeclarator));
		*decl->encl = *encl;
	}
	if (astHasErr()) {
		freeASTArrayDecl(decl);
		return 0;
	}
	
	if (tok[n].type == TT_O_BRACE) {
		n++;
	} else {
		freeASTArrayDecl(decl);
		return 0;
	}

	ASTNodeBuf tempBuf;
	ASTNode *tempNode = (ASTNode *) &tempBuf;

	if ((res = parseASTExp(tempNode, tok + n, scope))) {
		decl->exp = malloc(AST_NODE_S);
		mvASTNode(decl->exp, tempNode);
		n += res;
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		astErr("Expecting ] at end of array declaration", tok + n);
		freeASTArrayDecl(decl);
		return 0;
	}

	return n;
}

int printASTArrayDecl(const ASTArrayDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"Node Type\": \"ArrayDecl\"");

	if (decl->exp) {
		n += printf(", \"size\": ");
		n += printASTNode(decl->exp);
	}

	if (decl->encl) {
		n += printf(", \"enclosing type\": ");
		n += printASTDeclarator(decl->encl);
	}

	n += printf("}");

	return n;
}
