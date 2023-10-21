#include "for.h"
#include "astUtil.h"
#include "declaration.h"
#include "expression.h"
#include "node.h"
#include "statement.h"

void initASTFor(ASTFor *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	node->initClause = NULL;
	node->condExp = NULL;
	node->iterExp = NULL;
	node->loopStm = NULL;
}

void freeASTFor(ASTFor *node) {
	if (node->initClause) {
		freeASTNode(node->initClause);
		free(node->initClause);
	}

	if (node->condExp) {
		freeASTNode(node->condExp);
		free(node->condExp);
	}

	if (node->iterExp) {
		freeASTNode(node->iterExp);
		free(node->iterExp);
	}

	if (node->loopStm) {
		freeASTNode(node->loopStm);
		free(node->loopStm);
	}
}

int parseASTFor(
		ASTFor *node,
		const Token *tok,
		const ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFor(node, tok);
	if (astHasErr()) {
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_FOR) {
		n++;
	} else {
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("Expecting ( after while", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);

		if (tok[n].type == TT_SEMI_COLON) {
			n++;
		} else {
			astErr("Expecting ; after equation", tok + n);
			freeASTFor(node);
			return 0;
		}
	} else if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting expression or declaration following (", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->condExp = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting equation after ;", tok + n);
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		astErr("Expecting ; after cond expression", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->iterExp = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting iter expression after ;", tok + n);
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) after iter expression", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTStm((ASTStm *) &tempBuf, tok + n, scope))) {
		n += res;
		node->loopStm = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting statement after )", tok + n);
		freeASTFor(node);
		return 0;
	}

	node->node.type = AST_FOR;
	return n;
}

int printASTFor(ASTFor const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	if (node->initClause) {
		n += printf(", \"init clause\": ");
		n += printASTNode(node->initClause);
	}

	if (node->condExp) {
		n += printf(", \"condition expression\": ");
		n += printASTNode(node->condExp);
	}

	if (node->iterExp) {
		n += printf(", \"iteration expression\": ");
		n += printASTNode(node->iterExp);
	}

	if (node->loopStm) {
		n += printf(", \"loop statement\": ");
		n += printASTNode(node->loopStm);
	}

	n += printf("}");

	return n;
}

ASTTravRes astForTrav(
		ASTFor *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc)
{
	ASTTravRes result;

	result = astNodeTrav(node->initClause, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav(node->condExp, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav(node->iterExp, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav(node->loopStm, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}
