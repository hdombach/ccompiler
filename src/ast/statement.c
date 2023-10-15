#include <stdlib.h>

#include "doWhile.h"
#include "node.h"
#include "statement.h"
#include "compStatement.h"
#include "expression.h"
#include "astUtil.h"
#include "label.h"
#include "scope.h"
#include "if.h"
#include "switch.h"
#include "while.h"

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

void initASTEmptyStm(ASTEmptyStm *node) {
	initASTNode((ASTNode *) node);
}

int parseASTEmptyStm(
		ASTEmptyStm *node,
		const Token *tok,
		const struct ASTScope *scope)
{
	AST_VALID(ASTEmptyStm);
	int n = 0;

	initASTEmptyStm(node);
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

/***********************************************************************
 * Break Statement
 ***********************************************************************/

void initASTBreak(ASTBreak *node) {
	initASTNode((ASTNode *) node);
}

int parseASTBreak(
		ASTBreak *node,
		const Token *tok,
		const struct ASTScope *scope)
{
	AST_VALID(ASTBreak);
	int n = 0;

	initASTBreak(node);
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

void initASTContinue(ASTContinue *node) {
	initASTNode((ASTNode *) node);
}

int parseASTContinue(
		ASTContinue *node,
		Token const *tok,
		struct ASTScope const *scope)
{
	AST_VALID(ASTContinue);
	int n = 0;

	initASTContinue(node);
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

void initASTStm(ASTStm *node) {
	initASTNode((ASTNode *) node);
	node->type = ASTS_UNKNOWN;
	node->label = NULL;
}

void freeASTStm(ASTStm *node) {
	switch (node->type) {
		case ASTS_COMPOUND:
			freeASTNode(node->c.compStm);
			free(node->c.compStm);
			break;
		case ASTS_NODE:
			freeASTNode(node->c.node);
			free(node->c.node);
			break;
		case ASTS_IF:
			if (node->c.ifStm) {
				freeASTIf((ASTIf *) node->c.ifStm);
				free(node->c.ifStm);
			}
			break;
		case ASTS_SWITCH:
			if (node->c.switchStm) {
				freeASTNode(node->c.switchStm);
				free(node->c.switchStm);
			}
			break;
		case ASTS_WHILE:
			if (node->c.whileStm) {
				freeASTNode(node->c.whileStm);
				free(node->c.whileStm);
			}
			break;
		case ASTS_DO_WHILE:
			if (node->c.doWhileStm) {
				freeASTNode(node->c.doWhileStm);
				free(node->c.doWhileStm);
			}
			break;
		default:
			break;
	}

	if (node->label) {
		freeASTLabel(node->label);
		free(node->label);
		node->label = NULL;
	}
	node->type = ASTS_UNKNOWN;
}

int parseASTStm(ASTStm *node, const Token *tok, ASTScope const *scope) {
	AST_VALID(ASTStm);
	int res, n = 0;
	ASTLabel tempLabel;
	ASTNodeBuf tempBuf;

	initASTStm(node);
	if (astHasErr()) {
		freeASTStm(node);
		return 0;
	}

	if ((res = parseASTLabel(&tempLabel, tok + n, scope))) {
		node->label = malloc(sizeof(ASTLabel));
		*node->label = tempLabel;
		n += res;
	}

	if ((res = parseASTCompStm((ASTCompStm *) &tempBuf, tok + n, scope))) {
		node->c.compStm = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_COMPOUND;
		n += res;
	} else if ((res = parseASTIf((ASTIf *) &tempBuf, tok + n, scope))) {
		node->c.ifStm = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_IF;
		n += res;
	} else if ((res = parseASTSwitch((ASTSwitch *) &tempBuf, tok + n, scope))) {
		node->c.switchStm = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_SWITCH;
		n += res;
	} else if ((res = parseASTWhile((ASTWhile *) &tempBuf, tok + n, scope))) {
		node->c.whileStm = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_WHILE;
		n += res;
	} else if ((res = parseASTDoWhile((ASTDoWhile *) &tempBuf, tok + n, scope))) {
		node->c.doWhileStm = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_DO_WHILE;
		n += res;
	} else if ((res = parseASTBreak((ASTBreak *) &tempBuf, tok + n, scope))) {
		node->c.node = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_BREAK;
		n += res;
	} else if ((res = parseASTContinue((ASTBreak *) &tempBuf, tok + n, scope))) {
		node->c.node = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_CONTINUE;
		n += res;
	} else if ((res = parseASTEmptyStm((ASTEmptyStm *) &tempBuf, tok + n, scope))) {
		node->c.node = dupASTNode((ASTNode *) &tempBuf);
		node->type = ASTS_EMPTY;
		n += res;
	} else if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTStm(node);
			return 0;
		}
		n++;
		node->type = ASTS_NODE;
		node->c.node = dupASTNode((ASTNode *) &tempBuf);
	}

	node->node.type = AST_STM;
	return n;
}

int printASTStm(ASTStm const *node) {
	int n = 0;

	if (node->label) {
		n += printf("{");
		n += printf("\"node type\": \"statement\"");

		n += printf(", \"label\": ");
		n += printASTLabel(node->label);

		n += printf(", \"content\": ");
	}

	switch (node->type) {
		case ASTS_COMPOUND:
			n += printASTNode(node->c.compStm);
			break;
		case ASTS_EMPTY:
		case ASTS_BREAK:
		case ASTS_CONTINUE:
		case ASTS_NODE:
			n += printASTNode(node->c.node);
			break;
		case ASTS_IF:
			n += printASTNode(node->c.ifStm);
			break;
		case ASTS_SWITCH:
			n += printASTNode(node->c.switchStm);
			break;
		case ASTS_WHILE:
			n += printASTNode(node->c.whileStm);
			break;
		case ASTS_DO_WHILE:
			n += printASTNode(node->c.doWhileStm);
			break;
		default:
			n += printf("{\"type\": \"Statement\", \"value\": \"unknown\"}");
			break;
	}

	if (node->label) {
		n += printf("}");
	}

	return n;
}
