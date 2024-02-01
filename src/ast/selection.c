#include <stdio.h>
#include <stdlib.h>

#include "astUtil.h"
#include "selection.h"
#include "node.h"
#include "operation.h"
#include "statement.h"
#include "../tok/token.h"
#include "../util/log.h"

static ASTNodeVTable _ifVTable = {
	{
		(FreeFunc) freeASTIf,
		(PrintFunc) printASTIf,
	},
	(ASTChildCount) astIfChildCount,
	(ASTGetChild) astIfGetChild,
};

void initASTIf(ASTIf *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_ifVTable);
	node->expression = NULL;
	node->trueStatement = NULL;
	node->falseStatement = NULL;
}

void freeASTIf(ASTIf *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->trueStatement) {
		freeASTNode(node->trueStatement);
		free(node->trueStatement);
	}

	if (node->falseStatement) {
		freeASTNode(node->falseStatement);
		free(node->falseStatement);
	}
}

int parseASTIf(ASTIf *node, struct Token const *tok, struct ASTScope *scope) {
	AST_VALID(ASTIf);
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTIf(node, tok);

	if (astHasErr()) {
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_IF) {
		n++;
	} else {
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "expected (");
		freeASTIf(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_INV_EXP, tok + n, "expected expression");
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "expected )");
		freeASTIf(node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->trueStatement = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_EXP_STM, tok + n, "Expecting statement following expression");
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_ELSE) {
		n++;
	} else {
		node->node.type = AST_IF;
		return n;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->falseStatement = (ASTNode *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_EXP_STM, tok + n, "Expecting statement following else");
		freeASTIf(node);
		return 0;
	}

	node->node.type = AST_IF;

	return n;
}

int printASTIf(ASTIf const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"if statement\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->trueStatement) {
		n += printf(", \"true statement\": ");
		n += printASTNode(node->trueStatement);
	}

	if (node->falseStatement) {
		n += printf(", \"false statement\": ");
		n += printASTNode(node->falseStatement);
	}

	n += printf("}");

	return n;
}

int astIfChildCount(const ASTIf *node) {
	return 3;
}

ASTNode *astIfGetChild(ASTIf *node, int index) {
	return (ASTNode *[]) {
		(ASTNode *) node->expression,
		(ASTNode *) node->trueStatement,
		(ASTNode *) node->falseStatement,
	}[index];
}

/************************************************************
 * Switch 
 ************************************************************/

static ASTNodeVTable _switchVTable = {
	{
		(FreeFunc) freeASTSwitch,
		(PrintFunc) printASTSwitch,
	},
	(ASTChildCount) astSwitchChildCount,
	(ASTGetChild) astSwitchGetChild,
};

void initASTSwitch(ASTSwitch *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_switchVTable);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTSwitch(ASTSwitch *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTNode(node->statement);
		free(node->statement);
	}
}

int parseASTSwitch(ASTSwitch *node, Token const *tok, struct ASTScope *scope) {
	AST_VALID(ASTSwitch);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTSwitch(node, tok);
	if (astHasErr()) {
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_SWITCH) {
		n++;
	} else {
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expecting ( following switch");
		freeASTSwitch(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_INV_EXP, tok + n, "Expecting expression following switch");
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expected ) following switch");
		freeASTSwitch(node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = (ASTNode *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_INV_EXP, tok + n, "Expecting statement following switch");
		freeASTSwitch(node);
		return 0;
	}

	node->node.type = AST_SWITCH;

	return n;
}

int printASTSwitch(const ASTSwitch *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"switch\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTNode(node->statement);
	}

	n += printf("}");

	return n;
}

int astSwitchChildCount(ASTSwitch *node) {
	return 2;
}

ASTNode *astSwitchGetChild(ASTSwitch *node, int index) {
	return (ASTNode *[]) {
		(ASTNode *) node->expression,
		(ASTNode *) node->statement,
	}[index];
}
