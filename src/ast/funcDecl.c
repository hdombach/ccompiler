#include <stdlib.h>

#include "funcDecl.h"
#include "astUtil.h"
#include "param.h"

void initASTFuncDecl(ASTFuncDecl *decl) {
	initDList(&decl->params, sizeof(ASTParam));
	decl->encl = NULL;
	decl->hasEllipses = 0;
}

void freeASTFuncDecl(ASTFuncDecl *decl) {
	freeDList(&decl->params, (FreeFunc) freeASTParam);
	if (decl->encl) {
		freeASTDeclarator(decl->encl);
	}
}

int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		struct ASTDeclarator *encl)
{
	int n = 0, res;

	initASTFuncDecl(decl);
	if (encl) {
		decl->encl = malloc(sizeof(ASTDeclarator));
		*decl->encl = *encl;
	}
	if (astHasErr()) {
		freeASTFuncDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTFuncDecl(decl);
		return 0;
	}

	while (1) {
		ASTParam param;
		if ((res = parseASTParam(&param, tok + n, NULL))) {
			dlistApp(&decl->params, &param);
			n += res;
		} else if (tok[n].type == TT_DOTS) {
			decl->hasEllipses = 1;
			n++;
		} else {
			freeASTFuncDecl(decl);
			return 0;
		}

		if (tok[n].type == TT_COMMA) {
			n++;
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		freeASTFuncDecl(decl);
		return 0;
	}

	return n;
}

int printASTFuncDecl(const ASTFuncDecl *decl) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (decl->params.size) {
		n += printf("\"params\": ");
		printDList(&decl->params, (PrintFunc) printASTParam);
		isFirst = 0;
	}

	if (decl->hasEllipses) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"Has ellipses\": true");
	}

	if (decl->encl) {
		if (isFirst) {
			isFirst = 0;
		} else {
			n += printf(", ");
		}
		n += printf("\"enclosing type\": ");
		n += printASTDeclarator(decl->encl);
	}

	n += printf("}");

	return n;
}
