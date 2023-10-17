#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declaration.h"
#include "node.h"
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
	initASTNode((ASTNode *) node);
	node->func = NULL;
	initDListEmpty(&node->params, AST_NODE_S);
}

void freeASTFuncOperation(ASTFuncOperation *node) {
	if (node->func) {
		freeASTNode(node->func);
		free(node->func);
	}
	freeDList(&node->params, (FreeFunc) freeASTNode);
}

int parseASTFuncOperation(
		ASTFuncOperation *node,
		const Token *tok,
		ASTNode *func,
		ASTScope const *scope)
{
	AST_VALID(ASTFuncOperation);
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
			ASTNodeBuf tempBuf;
			if ((res = parseASTExp14((ASTNode *) &tempBuf, tok + n, scope))) {
				dlistApp(&node->params, (ASTNode *) &tempBuf);
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

	node->func = dupASTNode((ASTNode *) func);

	node->node.type = AST_FUNC_OPERATION;

	return n;
}

int printASTFuncOperation(ASTFuncOperation const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func operation\"");

	if (node->func) {
		n += printf(", \"func\": ");
		n += printASTNode(node->func);
	}

	n += printf(", \"params\": ");
	n += printDList(&node->params, (PrintFunc) printASTNode);

	n += printf("}");

	return n;
}

/*************************************************************
 * Subscript Operation
 *************************************************************/

int _parseASTSubscriptOperation(
		ASTOperation *node,
		const Token *tok,
		ASTNode *lhs,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_O_BRACE) {
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		node->rhs = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	node->lhs = dupASTNode(lhs);
	node->node.type = AST_SUBS_OPERATION;

	return n;
}

/*************************************************************
 * Condition Operation (ternary)
 *************************************************************/

void initASTCondOperation(ASTCondOperation *node) {
	initASTNode((ASTNode *) node);
	node->condition = NULL;
	node->trueExp = NULL;
	node->falseExp = NULL;
}

void freeASTCondOperation(ASTCondOperation *node) {
	if (node->condition) {
		freeASTNode(node->condition);
		free(node->condition);
		node->condition = NULL;
	}

	if (node->trueExp) {
		freeASTNode(node->trueExp);
		free(node->trueExp);
		node->trueExp = NULL;
	}

	if (node->falseExp) {
		freeASTNode(node->falseExp);
		free(node->falseExp);
		node->falseExp = NULL;
	}
}

int parseASTCondOperation(
		ASTCondOperation *node,
		Token const *tok,
		ASTScope const *scope)
{
	AST_VALID(ASTCondOperation);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTCondOperation(node);
	if (astHasErr()) {
		freeASTCondOperation(node);
		return 0;
	}

	if ((res = parseASTExp12((ASTNode *) &tempBuf, tok + n, scope))) {
		node->condition = dupASTNode((ASTNode *) &tempBuf);
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

	if ((res = parseASTExp12((ASTNode *) &tempBuf, tok + n, scope))) {
		node->trueExp = dupASTNode((ASTNode *) &tempBuf);
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

	if ((res = parseASTExp12((ASTNode *) &tempBuf, tok + n, scope))) {
		node->falseExp = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTCondOperation(node);
		return 0;
	}

	node->node.type = AST_COND_OPERATION;

	return n;
}

int printASTCondOperation(ASTCondOperation const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"conditional operation\"");

	if (node->condition) {
		n += printf(", \"condition\": ");
		n += printASTNode(node->condition);
	}

	if (node->trueExp) {
		n += printf(", \"true expression\": ");
		n += printASTNode(node->trueExp);
	}

	if (node->falseExp) {
		n += printf(", \"false expression\": ");
		n += printASTNode(node->falseExp);
	}

	n += printf("}");

	return n;
}

/*************************************************************
 * Type cast Operation
 *************************************************************/

int _parseASTCastOperation(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTOperation(node);
	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTParam((ASTParam *) &tempBuf, tok + n, scope))) {
		n += res;
		node->lhs = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTExp1((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->rhs = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTOperation(node);
		return 0;
	}

	node->node.type = AST_CAST_OPERATION;

	return n;
}

/*************************************************************
 * Sizeof Operation
 *************************************************************/

int _parseASTSizeofOperation(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope)
{
	AST_VALID(ASTOperation);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_SIZEOF) {
		n++;
	} else {
		freeASTOperation(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;

		if ((res = parseASTParam((ASTParam *) &tempBuf, tok + n, scope))) {
			n += res;
			node->rhs = dupASTNode((ASTNode *) &tempBuf);
		} else {
			freeASTOperation(node);
			return 0;
		}

		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTOperation(node);
			return 0;
		}
		node->node.type = AST_SIZEOF_TYPE_OPERATION;
	} else {
		if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
			n += res;
			node->rhs = dupASTNode((ASTNode *) &tempBuf);
		} else {
			freeASTOperation(node);
			return 0;
		}
		node->node.type = AST_SIZEOF_EXP_OPERATION;
	}

	return n;
}

/*************************************************************
 * Operation
 *************************************************************/

void initASTOperation(ASTOperation *node) {
	initASTNode(&node->node);
	node->tokType = TT_UNKNOWN;
	node->lhs = NULL;
	node->rhs = NULL;
}

void freeASTOperation(ASTOperation *node) {
	if (node->lhs) {
		freeASTNode(node->lhs);
		free(node->lhs);
	}
	if (node->rhs) {
		freeASTNode(node->rhs);
		free(node->rhs);
	}
	node->node.type = AST_UNKNOWN;
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
	(ASTNode *, const Token *, ASTScope const *scope);
int _parseASTOperationBin(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope,
		TokenType types[],
		_ParseOperationFunc lhsFunc,
		_ParseOperationFunc rhsFunc)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;
	TokenType tempType;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if ((res = lhsFunc((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->lhs = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTOperation(node);
		return 0;
	}

	if (_isTokenType(tok[n].type, types)) {
		tempType = tok[n].type;
		n++;
	} else {
		//makes code faster by not redoing work
		ASTNode *tempLhs = node->lhs;
		mvASTNode((ASTNode *) node, tempLhs);
		free(tempLhs);
		return n;
	}

	if ((res = rhsFunc((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->rhs = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTOperation(node);
		return 0;
	}

	node->node.type = AST_BINARY_OPERATION;
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
	AST_VALID(ASTOperation);
	int res, n = 0;
	ASTNodeBuf tempBuf;
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

	if ((res = rhsFunc((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	node->node.type = AST_PREFIX_OPERATION;
	node->rhs = dupASTNode((ASTNode *) &tempBuf);
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

	if ((res = parseASTCondOperation((ASTCondOperation *) node, tok + n, scope))) {
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
	} else if ((res = _parseASTCastOperation(node, tok + n, scope))) {
		n += res;
	} else if ((res = _parseASTSizeofOperation(node, tok + n, scope))) {
		n += res;
	} else {
		return 0;
	}

	return n;
}

int parseASTOperation1(
		ASTOperation *node,
		const Token *tok,
		ASTScope const *scope,
		ASTNode *lhs)
{
	int n = 0, res;

	initASTOperation(node);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if (_isTokenType(tok[n].type, operation1UnaryTypes)) {
		node->node.type = AST_POSTFIX_OPERATION;
		node->lhs = dupASTNode(lhs);
		node->tokType = tok[n].type;
		n++;
	} else if (_isTokenType(tok[n].type, operation1BinTypes)) {
		ASTNodeBuf tempBuf;
		node->tokType = tok[n].type;
		n++;
		if ((res = parseASTExpSing((ASTNode *) &tempBuf, tok + n, scope))) {
			node->node.type = AST_BINARY_OPERATION;
			node->lhs = dupASTNode(lhs);
			node->rhs = dupASTNode((ASTNode *) &tempBuf);
			n += res;
		} else {
			freeASTOperation(node);
			return 0;
		}
	} else if ((res = parseASTFuncOperation(
					(ASTFuncOperation *) node,
					tok + n,
					lhs,
					scope)))
	{
		n += res;
	} else if ((res = _parseASTSubscriptOperation(
					node,
					tok + n,
					lhs,
					scope)))
	{
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	return n;
}

int printASTOperation(ASTOperation const *node) {
	int n = 0;

	n += printf("{");
	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	if (node->tokType != TT_UNKNOWN) {
		n += printf(", \"operand\": ");
		n += printJsonStr(tokTypeStr(node->tokType));
	}

	if (node->lhs) {
		n += printf(", \"lhs\": ");
		n += printASTNode(node->lhs);
	}

	if (node->rhs) {
		n += printf(", \"rhs\": ");
		n += printASTNode(node->rhs);
	}

	n += printf("}");

	return n;
}
