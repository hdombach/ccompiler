#include <stdlib.h>
#include <string.h>

#include "declaration.h"
#include "operation.h"
#include "expression.h"
#include "astUtil.h"
#include "../util/util.h"
#include "../token.h"
#include "param.h"
#include "structDecl.h"

/*************************************************************
 * Func Operation
 *************************************************************/

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
		ASTExp func,
		ASTScope const *scope)
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
			if ((res = parseASTExp14(&tempExp, tok + n, scope))) {
				dlistApp(&node->params, &tempExp);
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

	node->func = malloc(sizeof(ASTExp));
	*node->func = func;
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

/*************************************************************
 * Subscript Operation
 *************************************************************/

void initASTSubscriptOperation(ASTSubscriptOperation *node) {
	node->lhs = NULL;
	node->index = NULL;
}

void freeASTSubscriptOperation(ASTSubscriptOperation *node) {
	if (node->lhs) {
		freeASTExp(node->lhs);
		free(node->lhs);
		node->lhs = NULL;
	}
	if (node->index) {
		freeASTExp(node->index);
		free(node->index);
		node->index = NULL;
	}
}

int parseASTSubscriptOperation(
		ASTSubscriptOperation *node,
		const Token *tok,
		struct ASTExp lhs,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTExp tempExp;

	initASTSubscriptOperation(node);
	if (astHasErr()) {
		freeASTSubscriptOperation(node);
		return 0;
	}

	if (tok[n].type == TT_O_BRACE) {
		n++;
	} else {
		freeASTSubscriptOperation(node);
		return 0;
	}

	if ((res = parseASTExp(&tempExp, tok + n, scope))) {
		node->index = malloc(sizeof(ASTExp));
		*node->index = tempExp;
		n += res;
	} else {
		freeASTSubscriptOperation(node);
		return 0;
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		freeASTSubscriptOperation(node);
		return 0;
	}

	node->lhs = malloc(sizeof(ASTExp));
	*node->lhs = lhs;

	return n;
}

int printASTSubscriptOperation(const ASTSubscriptOperation *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"subscript operation\"");

	if (node->lhs) {
		n += printf(", \"lhs\": ");
		n += printASTExp(node->lhs);
	}

	if (node->index) {
		n += printf(", \"index\": ");
		n += printASTExp(node->index);
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Condition Operation (ternary)
 *************************************************************/

void initASTCondOperation(ASTCondOperation *node) {
	node->condition = NULL;
	node->trueExp = NULL;
	node->falseExp = NULL;
}

void freeASTCondOperation(ASTCondOperation *node) {
	if (node->condition) {
		freeASTExp(node->condition);
		free(node->condition);
		node->condition = NULL;
	}

	if (node->trueExp) {
		freeASTExp(node->trueExp);
		free(node->trueExp);
		node->trueExp = NULL;
	}

	if (node->falseExp) {
		freeASTExp(node->falseExp);
		free(node->falseExp);
		node->falseExp = NULL;
	}
}

int parseASTCondOperation(
		ASTCondOperation *node,
		Token const *tok,
		ASTScope const *scope)
{
	int n = 0, res;
	ASTExp condition, trueExp, falseExp;

	initASTCondOperation(node);
	if (astHasErr()) {
		freeASTCondOperation(node);
		return 0;
	}

	if ((res = parseASTExp12(&condition, tok + n, scope))) {
		node->condition = malloc(sizeof(ASTExp));
		*node->condition = condition;
		n += res;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	if (tok[n].type == TT_QUESTION) {
		n++;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	if ((res = parseASTExp12(&trueExp, tok + n, scope))) {
		node->trueExp = malloc(sizeof(ASTExp));
		*node->trueExp = trueExp;
		n += res;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	if ((res = parseASTExp12(&falseExp, tok + n, scope))) {
		node->falseExp = malloc(sizeof(ASTExp));
		*node->falseExp = falseExp;
		n += res;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	return n;
}

int printASTCondOperation(ASTCondOperation const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"conditional operation\"");

	if (node->condition) {
		n += printf(", \"condition\": ");
		n += printASTExp(node->condition);
	}

	if (node->trueExp) {
		n += printf(", \"true expression\": ");
		n += printASTExp(node->trueExp);
	}

	if (node->falseExp) {
		n += printf(", \"false expression\": ");
		n += printASTExp(node->falseExp);
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Type cast Operation
 *************************************************************/

void initASTCastOperation(ASTCastOperation *node) {
	node->type = NULL;
	node->exp = NULL;
}

void freeASTCastOperation(ASTCastOperation *node) {
	if (node->type) {
		freeASTParam(node->type);
		free(node->type);
	}

	if (node->exp) {
		freeASTExp(node->exp);
		free(node->exp);
	}
}

int parseASTCastOperation(
		ASTCastOperation *node,
		const Token *tok,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTParam tempParam;
	ASTExp tempExp;

	initASTCastOperation(node);
	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTCastOperation(node);
		return 0;
	}

	if ((res = parseASTParam(&tempParam, tok + n, scope))) {
		n += res;
		node->type = malloc(sizeof(ASTDeclaration));
		*node->type = tempParam;
	} else {
		freeASTCastOperation(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		freeASTCastOperation(node);
		return 0;
	}

	if ((res = parseASTExp1(&tempExp, tok + n, scope))) {
		n += res;
		node->exp = malloc(sizeof(ASTExp));
		*node->exp = tempExp;
	} else {
		freeASTCastOperation(node);
		return 0;
	}

	return n;
}

int printASTCastOperation(const ASTCastOperation *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"cast operation\"");

	if (node->type) {
		n += printf(", \"type\": ");
		n += printASTParam(node->type);
	}

	if (node->exp) {
		n += printf(", \"expression\": ");
		n += printASTExp(node->exp);
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Sizeof Operation
 *************************************************************/

void initASTSizeofOperation(ASTSizeofOperation *node) {
	node->c.exp = NULL; // will also mean the param is NULL
}

void freeASTSizeofOperation(ASTSizeofOperation *node) {
	if (node->isExp) {
		if (node->c.exp) {
			freeASTExp(node->c.exp);
			free(node->c.exp);
		}
	} else {
		if (node->c.param) {
			freeASTParam(node->c.param);
			free(node->c.param);
		}
	}
}

int parseASTSizeofOperation(
		ASTSizeofOperation *node,
		Token const *tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTParam tempParam;
	ASTExp tempExp;

	initASTSizeofOperation(node);
	if (astHasErr()) {
		freeASTSizeofOperation(node);
		return 0;
	}

	if (tok[n].type == TT_SIZEOF) {
		n++;
	} else {
		freeASTSizeofOperation(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;

		if ((res = parseASTParam(&tempParam, tok + n, scope))) {
			n += res;
			node->c.param = malloc(sizeof(ASTParam));
			*node->c.param = tempParam;
			node->isExp = 0;
		} else {
			freeASTSizeofOperation(node);
			return 0;
		}

		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTSizeofOperation(node);
			return 0;
		}
	} else {
		if ((res = parseASTExp(&tempExp, tok + n, scope))) {
			n += res;
			node->c.exp = malloc(sizeof(ASTExp));
			*node->c.exp = tempExp;
			node->isExp = 1;
		} else {
			freeASTSizeofOperation(node);
			return 0;
		}
	}

	return n;
}

int printASTSizeofOperation(const ASTSizeofOperation *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"sizeof operation\"");

	if (node->isExp) {
		if (node->c.exp) {
			n += printf(", \"expression\": ");
			n += printASTExp(node->c.exp);
		}
	} else {
		if (node->c.param) {
			n += printf(", \"type\": ");
			n += printASTParam(node->c.param);
		}
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Operation
 *************************************************************/

void initASTOperation(ASTOperation *node) {
	node->type = AST_OT_UNKNOWN;
	node->tokType = TT_UNKNOWN;
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
			freeASTCastOperation(&node->c.typeCast);
			break;
		case AST_OT_COND:
			freeASTCondOperation(&node->c.cond);
			break;
		case AST_OT_SIZEOF:
			freeASTSizeofOperation(&node->c.sizeofOp);
			break;
		case AST_OT_SUBSCRIPT:
			freeASTSubscriptOperation(&node->c.subscript);
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

TokenType operation12Types[] = {
	TT_DBLE_BAR,
	TT_UNKNOWN,
};

TokenType operation11Types[] = {
	TT_DBLE_AMP,
	TT_UNKNOWN,
};

TokenType operation10Types[] = {
	TT_BAR,
	TT_UNKNOWN,
};

TokenType operation9Types[] = {
	TT_CARET,
	TT_UNKNOWN,
};

TokenType operation8Types[] = {
	TT_AMP,
	TT_UNKNOWN,
};

TokenType operation7Types[] = {
	TT_DBLE_EQL,
	TT_NOT_EQL,
	TT_UNKNOWN,
};

TokenType operation6Types[] = {
	TT_LESS,
	TT_LESS_EQL,
	TT_GREATER,
	TT_GREATER_EQL,
	TT_UNKNOWN,
};

TokenType operation5Types[] = {
	TT_DBLE_LESS,
	TT_DBLE_GREATER,
	TT_UNKNOWN,
};

TokenType operation4Types[] = {
	TT_PLUS,
	TT_MINUS,
	TT_UNKNOWN,
};

TokenType operation3Types[] = {
	TT_MULT,
	TT_DIV,
	TT_PERC,
	TT_UNKNOWN,
};

TokenType operation2Types[] = {
	TT_INC,
	TT_DEC,
	TT_PLUS,
	TT_MINUS,
	TT_EXCLAIM,
	TT_TILDE,
	TT_MULT,
	TT_AMP,
	TT_UNKNOWN,
};

TokenType operation1UnaryTypes[] = {
	TT_INC,
	TT_DEC,
	TT_UNKNOWN,
};

TokenType operation1BinTypes[] = {
	TT_PERIOD,
	TT_ARROW,
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

typedef int (*_ParseOperationFunc)
	(ASTExp *, const Token *, ASTScope const *scope);
int _parseASTOperationBin(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope,
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

	if ((res = lhsFunc(&lhs, tok + n, scope))) {
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
		if (lhs.type == ASTE_OPERATION) {
			*node = lhs.c.operation;
			return n;
		}
		freeASTExp(&lhs);
		freeASTOperation(node);
		return 0;
	}

	if ((res = rhsFunc(&rhs, tok + n, scope))) {
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

int _parseASTOperationPref(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope,
		TokenType types[],
		_ParseOperationFunc rhsFunc)
{
	int res, n = 0;
	ASTExp rhs;
	TokenType tempType;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if (_isTokenType(tok[n].type, types)) {
		tempType = tok[n].type;
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if ((res = rhsFunc(&rhs, tok + n, scope))) {
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	node->type = AST_OT_PREFIX;
	node->c.unary = malloc(sizeof(ASTExp));
	*node->c.unary = rhs;
	node->tokType = tempType;

	return n;
}

int parseASTOperation15(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation15Types,
			(_ParseOperationFunc) parseASTExp14,
			(_ParseOperationFunc) parseASTExp15);
}

int parseASTOperation14(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation14Types,
			(_ParseOperationFunc) parseASTExp13,
			(_ParseOperationFunc) parseASTExp14);
}

int parseASTOperation13(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;
	ASTCondOperation condOperation;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTCondOperation(&node->c.cond, tok + n, scope))) {
		node->type = AST_OT_COND;
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	return n;
}

int parseASTOperation12(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node, 
			tok, 
			scope,
			operation12Types, 
			(_ParseOperationFunc) parseASTExp11,
			(_ParseOperationFunc) parseASTExp12);
}

int parseASTOperation11(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation11Types,
			(_ParseOperationFunc) parseASTExp10,
			(_ParseOperationFunc) parseASTExp11);
}

int parseASTOperation10(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok, 
			scope,
			operation10Types, 
			(_ParseOperationFunc) parseASTExp9,
			(_ParseOperationFunc) parseASTExp10);
}

int parseASTOperation9(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation9Types,
			(_ParseOperationFunc) parseASTExp8,
			(_ParseOperationFunc) parseASTExp9);
}

int parseASTOperation8(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation8Types,
			(_ParseOperationFunc) parseASTExp7,
			(_ParseOperationFunc) parseASTExp8);
}

int parseASTOperation7(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation7Types,
			(_ParseOperationFunc) parseASTExp6,
			(_ParseOperationFunc) parseASTExp7);
}

int parseASTOperation6(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation6Types,
			(_ParseOperationFunc) parseASTExp5,
			(_ParseOperationFunc) parseASTExp6);
}

int parseASTOperation5(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation5Types,
			(_ParseOperationFunc) parseASTExp4,
			(_ParseOperationFunc) parseASTExp5);
}

int parseASTOperation4(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation4Types,
			(_ParseOperationFunc) parseASTExp3,
			(_ParseOperationFunc) parseASTExp4);
}

int parseASTOperation3(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation3Types,
			(_ParseOperationFunc) parseASTExp2,
			(_ParseOperationFunc) parseASTExp3);
}

int parseASTOperation2(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;

	if ((res = _parseASTOperationPref(
		node,
		tok + n,
		scope,
		operation2Types,
		(_ParseOperationFunc) parseASTExp2)))
	{
		n += res;
	} else if ((res = parseASTCastOperation(&node->c.typeCast, tok + n, scope))) {
		n += res;
		node->type = AST_OT_TYPECAST;
	} else if ((res = parseASTSizeofOperation(&node->c.sizeofOp, tok + n, scope))) {
		n += res;
		node->type = AST_OT_SIZEOF;
	} else {
		return 0;
	}

	return n;
}

int parseASTOperation1(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope,
		ASTExp lhs)
{
	int n = 0, res;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if (_isTokenType(tok[n].type, operation1UnaryTypes)) {
		node->type = AST_OT_POSTFIX;
		node->c.unary = malloc(sizeof(ASTExp));
		*node->c.unary = lhs;
		node->tokType = tok[n].type;
		n++;
	} else if (_isTokenType(tok[n].type, operation1BinTypes)) {
		ASTExp tempExp;
		node->tokType = tok[n].type;
		n++;
		if ((res = parseASTExpSing(&tempExp, tok + n, scope))) {
			node->type = AST_OT_BINARY;
			node->c.bin.lhs = malloc(sizeof(ASTExp));
			*node->c.bin.lhs = lhs;
			node->c.bin.rhs = malloc(sizeof(ASTExp));
			*node->c.bin.rhs = tempExp;
			n += res;
		} else {
			freeASTOperation(node);
			return 0;
		}
	} else if ((res = parseASTFuncOperation(
					&node->c.func,
					tok + n,
					lhs,
					scope)))
	{
		node->type = AST_OT_FUNC;
		n += res;
	} else if ((res = parseASTSubscriptOperation(
					&node->c.subscript,
					tok + n,
					lhs,
					scope)))
	{
		node->type = AST_OT_SUBSCRIPT;
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	return n;
}

int printASTOperation(ASTOperation const *node) {
	int n = 0;

	switch (node->type) {
		case AST_OT_BINARY:
			n += printf("{");
			n += printf("\"node type\": \"operation\"");

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"lhs\": ");
			n += printASTExp(node->c.bin.lhs);

			n += printf(", \"rhs\": ");
			n += printASTExp(node->c.bin.rhs);

			n += printf("}");

			break;

		case AST_OT_PREFIX:
			n += printf("{");
			n += printf("\"node type\": \"operation\"");

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"rhs\": ");
			n += printASTExp(node->c.unary);

			n += printf("}");

			break;

		case AST_OT_POSTFIX:
			n += printf("{");
			n += printf("\"node type\": \"operation\"");

			n += printf(", \"operand\": ");
			n += printJsonStr(tokTypeStr(node->tokType));

			n += printf(", \"lhs\": ");
			n += printASTExp(node->c.unary);

			n += printf("}");

			break;

		case AST_OT_FUNC:
			n += printASTFuncOperation(&node->c.func);
			break;

		case AST_OT_TYPECAST:
			n += printASTCastOperation(&node->c.typeCast);
			break;

		case AST_OT_COND:
			n += printASTCondOperation(&node->c.cond);
			break;

		case AST_OT_SIZEOF:
			n += printASTSizeofOperation(&node->c.sizeofOp);
			break;

		case AST_OT_SUBSCRIPT:
			n += printASTSubscriptOperation(&node->c.subscript);
			break;

		default:
			break;
	}

	return n;
}
