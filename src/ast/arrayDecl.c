#include <stdlib.h>

#include "arrayDecl.h"
#include "declaration.h"
#include "expression.h"
#include "astUtil.h"
#include "structDecl.h"

void initASTArrayDecl(ASTArrayDecl *decl) {
	decl->exp = NULL;
	decl->encl = NULL;
}

void freeASTArrayDecl(ASTArrayDecl *decl) {
	if (decl->exp) {
		freeASTExp(decl->exp);
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

	ASTExp tempExp;
	if ((res = parseASTExp(&tempExp, tok + n, scope))) {
		decl->exp = malloc(sizeof(ASTExp));
		*decl->exp = tempExp;
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
		n += printASTExp(decl->exp);
	}

	if (decl->encl) {
		n += printf(", \"enclosing type\": ");
		n += printASTDeclarator(decl->encl);
	}

	n += printf("}");

	return n;
}
