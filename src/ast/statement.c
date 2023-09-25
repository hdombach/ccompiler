#include "statement.h"
#include "compStatement.h"
#include "expression.h"
#include "astUtil.h"
#include "label.h"
#include "scope.h"
#include "if.h"
#include <stdlib.h>

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
		case ASTS_EXP:
			freeASTExp(&node->c.exp);
			break;
		case ASTS_IF:
			freeASTIf(&node->c.ifStm);
			break;
		default:
			break;
	}

	if (node->label) {
		freeASTLabel(node->label);
		free(node->label);
		node->label = NULL;
	}
}

int parseASTStm(ASTStm *node, const Token *tok, ASTScope *scope) {
	int res, n = 0;
	ASTCompStm tempCompStm;
	ASTLabel tempLabel;

	if (astHasErr()) {
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
	} else if ((res = parseASTExp(&node->c.exp, tok + n, scope))) {
		node->type = ASTS_EXP;
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTExp(&node->c.exp);
			return 0;
		}
		n++;
	} else if ((res = parseASTIf(&node->c.ifStm, tok + n, scope))) {
		node->type = ASTS_IF;
		n += res;
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
		case ASTS_EXP:
			n += printASTExp(&node->c.exp);
			break;
		case ASTS_IF:
			n += printASTIf(&node->c.ifStm);
			break;
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
