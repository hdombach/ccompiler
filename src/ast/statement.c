#include <stdlib.h>

#include "doWhile.h"
#include "statement.h"
#include "compStatement.h"
#include "expression.h"
#include "astUtil.h"
#include "label.h"
#include "scope.h"
#include "if.h"
#include "switch.h"
#include "while.h"

void initASTStm(ASTStm *node) {
	node->type = ASTS_UNKNOWN;
	node->label = NULL;
}

void freeASTStm(ASTStm *node) {
	switch (node->type) {
		case ASTS_COMPOUND:
			freeASTCompStm(node->c.compStm);
			free(node->c.compStm);
			break;
		case ASTS_NODE:
			freeASTNode((ASTNode *) &node->c.nodeBuf);
			break;
		case ASTS_IF:
			freeASTIf(&node->c.ifStm);
			break;
		case ASTS_SWITCH:
			freeASTSwitch(&node->c.switchStm);
			break;
		case ASTS_WHILE:
			freeASTWhile(&node->c.whileStm);
			break;
		case ASTS_DO_WHILE:
			freeASTDoWhile(&node->c.doWhileStm);
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
	int res, n = 0;
	ASTCompStm tempCompStm;
	ASTLabel tempLabel;

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

	if ((res = parseASTCompStm(&tempCompStm, tok + n, scope))) {
		n += res;
		node->type = ASTS_COMPOUND;
		node->c.compStm = malloc(sizeof(ASTCompStm));
		*node->c.compStm = tempCompStm;
	} else if ((res = parseASTIf(&node->c.ifStm, tok + n, scope))) {
		node->type = ASTS_IF;
		n += res;
	} else if ((res = parseASTSwitch(&node->c.switchStm, tok + n, scope))) {
		node->type = ASTS_SWITCH;
		n += res;
	} else if ((res = parseASTWhile(&node->c.whileStm, tok + n, scope))) {
		node->type = ASTS_WHILE;
		n += res;
	} else if ((res = parseASTDoWhile(&node->c.doWhileStm, tok + n, scope))) {
		node->type = ASTS_DO_WHILE;
		n += res;
	} else if (tok[n].type == TT_BREAK) {
		n++;
		if (tok[n].type == TT_SEMI_COLON) {
			n++;
		} else {
			freeASTStm(node);
			return 0;
		}
		node->type = ASTS_BREAK;
	} else if (tok[n].type == TT_CONTINUE) {
		n++;
		if (tok[n].type == TT_SEMI_COLON) {
			n++;
		} else {
			freeASTStm(node);
			return 0;
		}
	} else if (tok[n].type == TT_SEMI_COLON) {
		node->type = ASTS_EMPTY;
		n++;
	} else if ((res = parseASTExp((ASTNode *) &node->c.nodeBuf, tok + n, scope))) {
		node->type = ASTS_NODE;
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTNode(&node->c.nodeBuf);
			return 0;
		}
		n++;
	}

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
			n += printASTCompStm(node->c.compStm);
			break;
		case ASTS_NODE:
			n += printASTNode(&node->c.nodeBuf);
			break;
		case ASTS_IF:
			n += printASTIf(&node->c.ifStm);
			break;
		case ASTS_SWITCH:
			n += printASTSwitch(&node->c.switchStm);
			break;
		case ASTS_WHILE:
			n += printASTWhile(&node->c.whileStm);
			break;
		case ASTS_DO_WHILE:
			n += printASTDoWhile(&node->c.doWhileStm);
			break;
		case ASTS_EMPTY:
			n += printf("\"empty\"");
			break;
		case ASTS_BREAK:
			n += printf("\"break\"");
			break;
		case ASTS_CONTINUE:
			n += printf("\"continue\"");
		default:
			n += printf("{\"type\": \"Statement\", \"value\": \"unknown\"}");
			break;
	}

	if (node->label) {
		n += printf("}");
	}

	return n;
}
