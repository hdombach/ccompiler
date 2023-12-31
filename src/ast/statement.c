#include <stdlib.h>

#include "node.h"
#include "statement.h"
#include "compStatement.h"
#include "expression.h"
#include "astUtil.h"
#include "label.h"
#include "../sem/scope.h"
#include "selection.h"
#include "iteration.h"

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

void initASTEmptyStm(ASTEmptyStm *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
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

void initASTBreak(ASTBreak *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
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

void initASTContinue(ASTContinue *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
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

void initASTGoto(ASTGoto *node, const struct Token *tok) {
	initASTNode((ASTNode *) node, tok);
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

void initASTStm(ASTStm *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	node->label = NULL;
	node->content = NULL;
}

void freeASTStm(ASTStm *node) {
	if (node->content) {
		freeASTNode(node->content);
		free(node->content);
	}
	if (node->label) {
		freeASTNode(node->label);
		free(node->label);
		node->label = NULL;
	}
}

int parseASTStm(ASTStm *node, const Token *tok, ASTScope *scope) {
	AST_VALID(ASTStm);
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTStm(node, tok);
	if (astHasErr()) {
		freeASTStm(node);
		return 0;
	}

	if ((res = parseASTLabel((ASTNode *) &tempBuf, tok + n, scope))) {
		node->label = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	}

	if ((res = parseASTCompStm((ASTCompStm *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTIf((ASTIf *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTSwitch((ASTSwitch *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTWhile((ASTWhile *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTDoWhile((ASTDoWhile *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTBreak((ASTBreak *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTContinue((ASTBreak *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTEmptyStm((ASTEmptyStm *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTGoto((ASTGoto *) &tempBuf, tok + n, scope))) {
		node->content = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTStm(node);
			return 0;
		}
		n++;
		node->content = dupASTNode((ASTNode *) &tempBuf);
	}

	node->node.type = AST_STM;
	return n;
}

int printASTStm(ASTStm const *node) {
	int n = 0;

	if (node->label) {
		n += printf("{");
		n += printf("\"node type\": \"statement\"");

		if (node->label) {
			n += printf(", \"label\": ");
			n += printASTNode(node->label);
		}

		n += printf(", \"content\": ");
	}

	if (node->content) {
		n += printASTNode(node->content);
	}

	if (node->label) {
		n += printf("}");
	}

	return n;
}

int astStmChildCount(const ASTStm *node) {
	return 2;
}

ASTNode *astStmGetChild(ASTStm *node, int index) {
	return (ASTNode *[]) {
		node->content,
		node->label,
	}[index];
}
