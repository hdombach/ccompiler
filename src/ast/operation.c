#include <stdlib.h>

#include "declaration.h"
#include "operation.h"
#include "expression.h"
#include "astUtil.h"
#include "../util/util.h"
#include "../token.h"

void initASTFuncOperation(ASTFuncOperation *node) {
	node->func = NULL;
	initDListEmpty(&node->params, sizeof(ASTExp));
}

void freeASTFuncOperation(ASTFuncOperation *node) {
	if (node->func) {
		freeASTExp(node->func);
		free(node->func);
	}
	freeDList(&node->params, (FreeFunc) freeASTExp);
}

int parseASTFuncOperation(
		ASTFuncOperation *node,
		const Token *tok,
		ASTExp *func)
{
	int res, n = 0;

	initASTFuncOperation(node);
	if (astHasErr()) {
		freeASTFuncOperation(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTFuncOperation(node);
		return 0;
	}

	if (tok[n].type != TT_C_PARAN) {
		while (1) {
			ASTExp tempExp;
			if ((res = parseASTExp(&tempExp, tok + n))) {
				n += res;
			} else {
				freeASTFuncOperation(node);
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
		freeASTFuncOperation(node);
		return 0;
	}

	node->func = func;
	return n;
}

int printASTFuncOperation(ASTFuncOperation const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func operation\"");

	if (node->func) {
		n += printf(", \"func\": ");
		n += printASTExp(node->func);
	}

	n += printf(", \"params\": ");
	n += printDList(&node->params, (PrintFunc) printASTExp);

	n += printf("}");

	return n;
}

void initASTOperation(ASTOperation *node) {
	node->type = AST_OT_UNKNOWN;
}

void freeASTOperation(ASTOperation *node) {
	switch (node->type) {
		case AST_OT_BINARY:
			freeASTExp(node->c.bin.lhs);
			free(node->c.bin.lhs);
			freeASTExp(node->c.bin.rhs);
			free(node->c.bin.rhs);
			break;
		case AST_OT_PREFIX:
			freeASTExp(node->c.unary);
			free(node->c.unary);
			break;
		case AST_OT_POSTFIX:
			freeASTExp(node->c.unary);
			free(node->c.unary);
			break;
		case AST_OT_FUNC:
			freeASTFuncOperation(&node->c.func);
			break;
		case AST_OT_TYPECAST:
			freeASTDeclaration(node->c.typeCast);
			break;
		default:
			break;
	}
}

int parseASTOperation15(ASTOperation *node, const Token *tok) {
	int res, n = 0;
	ASTExp lhs, rhs;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTExp14(&lhs, tok + n))) {
		n += res;
	} else {
		freeASTExp(&lhs);
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_COMMA) {
		n++;
	} else {
		freeASTExp(&lhs);
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTExp15(&rhs, tok + n))) {
		n += res;
	} else {
		freeASTExp(&lhs);
		freeASTExp(&rhs);
		freeASTOperation(node);
		return 0;
	}

	node->type = AST_OT_BINARY;
	node->c.bin.lhs = malloc(sizeof(ASTExp));
	node->c.bin.rhs = malloc(sizeof(ASTExp));
	*node->c.bin.lhs = lhs;
	*node->c.bin.rhs = rhs;
	node->tokType = TT_COMMA;

	return n;
}

int printASTOperation(ASTOperation const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"operation\"");

	switch (node->type) {
		case AST_OT_BINARY:

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"lhs\": ");
			n += printASTExp(node->c.bin.lhs);

			n += printf(", \"rhs\": ");
			n += printASTExp(node->c.bin.rhs);

			break;

		case AST_OT_PREFIX:

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"lhs\": ");
			n += printASTExp(node->c.bin.lhs);

			break;

		case AST_OT_POSTFIX:

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"rhs\": ");
			n += printASTExp(node->c.bin.rhs);

			break;

		case AST_OT_FUNC:
			n += printASTFuncOperation(&node->c.func);
			break;

		case AST_OT_TYPECAST:
			n += printf(", \"operand\": \"type cast\"");

			n += printf(", type: ");
			n += printASTDeclaration(node->c.typeCast);

			break;

		default:
			break;
	}

	n += printf("}");

	return n;
}
