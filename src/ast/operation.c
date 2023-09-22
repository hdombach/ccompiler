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

TokenType operation15Types[] = {
	TT_COMMA,
	TT_UNKNOWN,
};

TokenType operation14Types[] = {
	TT_EQL,
	TT_PLUS_EQL,
	TT_MINUS_EQL,
	TT_MULT_EQL,
	TT_DIV_EQL,
	TT_PERC_EQL,
	TT_DBLE_LESS_EQL,
	TT_DBLE_GREATER_EQL,
	TT_AMP_EQL,
	TT_CARET_EQL,
	TT_BAR_EQL,
	TT_UNKNOWN,
};

int _isTokenType(TokenType type, TokenType types[]) {
	TokenType *curType = types;
	while (*curType != TT_UNKNOWN) {
		if (*curType == type) {
			return 1;
		}
		curType++;
	}
	return 0;
}

typedef int (*_ParseOperationFunc)(ASTExp *, const Token *);
int _parseASTOperationBin(
		ASTOperation *node,
		const Token *tok,
		TokenType types[],
		_ParseOperationFunc lhsFunc,
		_ParseOperationFunc rhsFunc)
{
	int res, n = 0;
	ASTExp lhs, rhs;
	TokenType tempType;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if ((res = lhsFunc(&lhs, tok + n))) {
		n += res;
	} else {
		freeASTExp(&lhs);
		freeASTOperation(node);
		return 0;
	}

	if (_isTokenType(tok[n].type, types)) {
		tempType = tok[n].type;
		n++;
	} else {
		freeASTExp(&lhs);
		freeASTOperation(node);
		return 0;
	}

	if ((res = rhsFunc(&rhs, tok + n))) {
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
	node->tokType = tempType;

	return n;
}

int parseASTOperation15(ASTOperation *node, const Token *tok) {
	return _parseASTOperationBin(
			node,
			tok,
			operation15Types,
			(_ParseOperationFunc) parseASTExp14,
			(_ParseOperationFunc) parseASTExp15);
}

int parseASTOperation14(ASTOperation *node, const Token *tok) {
	return _parseASTOperationBin(
			node,
			tok,
			operation14Types,
			(_ParseOperationFunc) parseASTExp13,
			(_ParseOperationFunc) parseASTExp14);
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
