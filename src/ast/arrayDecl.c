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
		ASTDeclarator *encl) {
	int n = 0, res;

	initASTArrayDecl(decl);
	decl->encl = malloc(sizeof(ASTDeclarator));
	*decl->encl = *encl;
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

	if (tok[n].type == TT_MULT) {
		n++;
	} else {
		decl->exp = malloc(sizeof(ASTExp));
		if ((res = parseASTExp(decl->exp, tok + n))) {
			n += res;
		} else {
			free(decl->exp);
			decl->exp = NULL;
			freeASTArrayDecl(decl);
			return 0;
		}
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		freeASTArrayDecl(decl);
		return 0;
	}

	return n;
}

int printASTArrayDecl(const ASTArrayDecl *decl) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (decl->exp) {
		n += printf("\"size\": ");
		n += printASTExp(decl->exp);
		isFirst = 0;
	}

	if (isFirst) {
		isFirst = 0;
	} else {
		n += printf(", ");
	}
	n += printf("\"enclosing type\": ");
	n += printASTDeclarator(decl->encl);

	n += printf("}");

	return n;
}
