#include "statement.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "compStatement.h"
#include "astUtil.h"
#include "label.h"
#include "../sem/scope.h"
#include "operation.h"
#include "selection.h"
#include "iteration.h"

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

static ASTNodeVTable _emptyVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTEmptyStm,
	},
	(ASTChildCount) astEmptyStmChildCount,
	(ASTGetChild) astEmptyStmGetChild,
};

void initASTEmptyStm(ASTEmptyStm *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_emptyVTable);
}

int parseASTEmptyStm(
		ASTEmptyStm *node,
		const Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTEmptyStm);
	int n = 0;

	initASTEmptyStm(node, tok);
	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		return 0;
	}

	node->type = AST_EMPTY_STM;
	return n;
}

int printASTEmptyStm(const ASTEmptyStm *node) {
	return printf("{\"node type\": \"%s\"}", astNodeTypeStr(node->type));
}

int astEmptyStmChildCount(const ASTEmptyStm *node) {
	return 0;
}

ASTNode *astEmptyStmGetChild(ASTEmptyStm *node, int index) {
	return NULL;
}

/***********************************************************************
 * Break Statement
 ***********************************************************************/

static ASTNodeVTable _breakVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTBreak,
	},
	(ASTChildCount) astBreakChildCount,
	(ASTGetChild) astBreakGetChild,
};

void initASTBreak(ASTBreak *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_breakVTable);
}

int parseASTBreak(
		ASTBreak *node,
		const Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTBreak);
	int n = 0;

	initASTBreak(node, tok);
	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type == TT_BREAK) {
		n++;
	} else {
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		return 0;
	}

	node->type = AST_BREAK;
	return n;
}

int printASTBreak(const ASTBreak *node) {
	return printf("{\"node type\": \"%s\"}", astNodeTypeStr(node->type));
}

int astBreakChildCount(const ASTBreak *node) {
	return 0;
}

ASTNode *astBreakGetChild(ASTBreak *node, int index) {
	return NULL;
}

/***********************************************************************
 * Continue Statement
 ***********************************************************************/

struct ASTNodeVTable _continueVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTContinue,
	},
	(ASTChildCount) astContinueChildCount,
	(ASTGetChild) astContinueGetChild,
};

void initASTContinue(ASTContinue *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_continueVTable);
}

int parseASTContinue(
		ASTContinue *node,
		Token const *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTContinue);
	int n = 0;

	initASTContinue(node, tok);
	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type == TT_CONTINUE) {
		n++;
	} else {
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		return 0;
	}

	node->type = AST_CONTINUE;
	return n;
}

int printASTContinue(const ASTContinue *node) {
	return printf("{\"node type\": \"%s\"}", astNodeTypeStr(node->type));
}

int astContinueChildCount(const ASTContinue *node) {
	return 0;
}

ASTNode *astContinueGetChild(ASTContinue *node, int index) {
	return NULL;
}

/***********************************************************************
 * Goto Statement
 ***********************************************************************/

static ASTNodeVTable _gotoVTable = {
	{
		(FreeFunc) freeASTGoto,
		(PrintFunc) printASTGoto,
	},
	(ASTChildCount) astGotoChildCount,
	(ASTGetChild) astGotoGetChild,
};

void initASTGoto(ASTGoto *node, const struct Token *tok) {
	initASTNode((ASTNode *) node, tok, &_gotoVTable);
	node->name = NULL;
}

void freeASTGoto(ASTGoto *node) {
	if (node->name) {
		free(node->name);
	}
}

int parseASTGoto(
		ASTGoto *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTGoto);
	int n = 0;

	initASTGoto(node, tok);
	if (astHasErr()) {
		freeASTGoto(node);
		return 0;
	}

	if (tok[n].type == TT_GOTO) {
		n++;
	} else {
		freeASTGoto(node);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		node->name = strdup(tok[n].contents);
		n++;
	} else {
		freeASTGoto(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		freeASTGoto(node);
		return 0;
	}

	node->node.type = AST_GOTO;
	return n;
}

int printASTGoto(const ASTGoto *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": ");
	n += printJsonStr(astNodeTypeStr(node->node.type));

	n += printf(", \"label\": ");
	n += printJsonStr(node->name);

	n += printf("}");

	return n;
}

int astGotoChildCount(ASTGoto *node) {
	return 0;
}

ASTNode *astGotoGetChild(ASTGoto *node, int index) {
	return NULL;
}

/***********************************************************************
 * Return Statement
 ***********************************************************************/

static ASTNodeVTable _returnTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTReturn,
	},
	(ASTChildCount) astReturnChildCount,
	(ASTGetChild) astReturnGetChild,
};

void initASTReturn(ASTReturn *node, const Token *tok) {
	node->value = NULL;
	initASTNode((ASTNode *) node, tok, &_returnTable);
}

void freeASTReturn(ASTReturn *node) {
	if (node->value) {
		freeASTNode(node->value);
		node->value = NULL;
	}
}

int parseASTReturn(
		ASTReturn *node,
		const Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTReturn);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTReturn(node, tok);
	if (astHasErr()) return 0;

	if (tok[n].type == TT_RETURN) {
		n++;
	} else {
		goto failure;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->value = dupASTNode((ASTNode *) &tempBuf);
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		goto failure;
	}

	node->node.type = AST_RETURN;

	return n;

failure:
	freeASTReturn(node);
	return 0;
}

int printASTReturn(const ASTReturn *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	if (node->value) {
		n += printf("\"return value\": ");
		n += printASTNode(node->value);
	}

	n += printf("}");

	return n;
}

int astReturnChildCount(ASTReturn *node) {
	if (node->value) {
		return 1;
	} else {
		return 0;
	}
}

ASTNode *astReturnGetChild(ASTReturn *node, int index) {
	if (index == 0 && node->value) {
		return node->value;
	} else {
		return NULL;
	}
}

/***********************************************************************
 * Statement Statement
 ***********************************************************************/

int parseASTStm(ASTNode *node, const Token *tok, ASTScope *scope) {
	int res, n = 0;

	if ((res = parseASTLbl(node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTCompStm((ASTCompStm *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTIf((ASTIf *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTSwitch((ASTSwitch *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTWhile((ASTWhile *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTDoWhile((ASTDoWhile *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTFor((ASTFor *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTBreak((ASTBreak *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTContinue((ASTBreak *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTReturn((ASTReturn *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTEmptyStm((ASTEmptyStm *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTGoto((ASTGoto *) node, tok + n, scope))) {
		return res;
	} else if ((res = parseASTOperation((ASTOperation *) node, tok + n, scope))) {
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTNode(node);
			return 0;
		}
		n++;
		return n;
	}
	return 0;
}
