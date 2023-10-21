#include <stdlib.h>

#include "funcDecl.h"
#include "astUtil.h"
#include "node.h"
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
		free(decl->encl);
	}
}

int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		struct ASTDeclarator *encl,
		struct ASTScope const *scope)
{
	AST_VALID(ASTFuncDecl);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFuncDecl(decl);
	if (encl) {
		decl->encl = encl;
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

	if (tok[n].type != TT_C_PARAN) {
		while (1) {
			if ((res = parseASTParam((ASTParam *) &tempBuf, tok + n, scope))) {
				dlistApp(&decl->params, &tempBuf);
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
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting )", tok + n);
		freeASTFuncDecl(decl);
		return 0;
	}

	decl->node.type = AST_FUNC_DECL;
	return n;
}

int printASTFuncDecl(const ASTFuncDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func decl\"");

	if (decl->params.size) {
		n += printf(", \"params\": ");
		printDList(&decl->params, (PrintFunc) printASTParam);
	}

	if (decl->hasEllipses) {
		n += printf(", \"Has ellipses\": true");
	}

	if (decl->encl) {
		n += printf(", \"enclosing type\": ");
		n += printASTDeclarator(decl->encl);
	}

	n += printf("}");

	return n;
}

ASTTravRes astFuncDeclTrav(ASTFuncDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc)
{
	ASTTravRes result;

	for (int i = 0; i < node->params.size; i++) {
		ASTNode *param = dlistGetm(&node->params, i);
		result = astNodeTrav(param, beforeFunc, afterFunc);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	result = astNodeTrav((ASTNode *) node->encl, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}
