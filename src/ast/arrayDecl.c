#include <stdlib.h>

#include "arrayDecl.h"
#include "declaration.h"
#include "expression.h"
#include "astUtil.h"
#include "node.h"
#include "structDecl.h"

void initASTArrayDecl(ASTArrayDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	decl->exp = NULL;
	decl->encl = NULL;
}

void freeASTArrayDecl(ASTArrayDecl *decl) {
	if (decl->exp) {
		freeASTNode(decl->exp);
		free(decl->exp);
	}
	if (decl->encl) {
		freeASTNode(decl->encl);
		free(decl->encl);
	}
}

int parseASTArrayDecl(
		ASTArrayDecl *decl,
		const Token *tok,
		ASTNode *encl,
		ASTScope const *scope) {
	AST_VALID(ASTArrayDecl);
	int n = 0, res;

	initASTArrayDecl(decl, tok);
	decl->node.tok = tok;
	if (encl) {
		decl->encl = encl;
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

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		decl->exp = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		astErr("Expecting ] at end of array declaration", tok + n);
		freeASTArrayDecl(decl);
		return 0;
	}

	decl->node.type = AST_ARRAY_DECL;

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
		n += printASTNode(decl->encl);
	}

	n += printf("}");

	return n;
}

ASTTravRes astArrayDeclTrav(
		ASTArrayDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	if (node->encl) {
		result = astNodeTrav(node->encl, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	if (node->exp) {
		result = astNodeTrav(node->exp, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}
