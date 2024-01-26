#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "declaration.h"
#include "node.h"
#include "operation.h"
#include "astUtil.h"
#include "../util/util.h"
#include "../tok/token.h"
#include "param.h"
#include "intConstant.h"
#include "floatConst.h"
#include "stringConstant.h"
#include "charConstant.h"


/**
 * @brief Parses constants or identifiers
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
static int _parseASTExpSing(
		ASTNode *node,
		Token const *tok,
		struct ASTScope *scope)
{
	int res, n = 0;
	initASTNode(node, tok, NULL);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTIntConstant((ASTIntConstant *) node, tok))) {
		return res;
	} else if ((res = parseASTFloatConstant((ASTFloatConstant *) node, tok))) {
		return res;
	} else if ((res = parseASTCharConstant((ASTCharConstant *) node, tok))) {
		return res;
	} else if ((res = parseASTStringConstant((ASTStringConstant *) node, tok))) {
		return res;
	} else if ((res = parseASTIdentifier((ASTIdentifier *) node, tok, scope))) {
		return res;
	} else if (tok[n].type == TT_O_PARAN) {
		n++;
		if ((res = parseASTOperation((ASTOperation *) node, tok + n, scope))) {
			n += res;
		} else {
			freeASTNode(node);
			return 0;
		}

		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTNode(node);
			return 0;
		}
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}



/*************************************************************
 * Func Operation
 *************************************************************/

static ASTNodeVTable _funcVTable = {
	{
		(FreeFunc) freeASTFuncOperation,
		(PrintFunc) printASTFuncOperation,
	},
	(ASTChildCount) astFuncOperationChildCount,
	(ASTGetChild) astFuncOperationGetChild,
};

void initASTFuncOperation(ASTFuncOperation *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_funcVTable);
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
		ASTScope *scope)
{
	AST_VALID(ASTFuncOperation);
	int res, n = 0;

	initASTFuncOperation(node, tok);
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
			if ((res = parseASTOperation14((ASTOperation *) &tempBuf, tok + n, scope))) {
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

int astFuncOperationChildCount(const ASTFuncOperation *node) {
	return node->params.size + 1;
}

ASTNode *astFuncOperationGetChild(ASTFuncOperation *node, int index) {
	if (index < 1) {
		return node->func;
	} else {
		return dlistGetm(&node->params, index - 1);
	}
}

/*************************************************************
 * Subscript Operation
 *************************************************************/

static int _parseASTSubscriptOperation(
		ASTOperation *node,
		const Token *tok,
		ASTNode *lhs,
		struct ASTScope *scope)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTOperation(node, tok);
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

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
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

static ASTNodeVTable _condVTable = {
	{
		(FreeFunc) freeASTCondOperation,
		(PrintFunc) printASTCondOperation,
	},
	(ASTChildCount) astCondOperationChildCount,
	(ASTGetChild) astCondOperationGetChild,
};

void initASTCondOperation(ASTCondOperation *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_condVTable);
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
		ASTScope *scope)
{
	AST_VALID(ASTCondOperation);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTCondOperation(node, tok);
	if (astHasErr()) {
		freeASTCondOperation(node);
		return 0;
	}

	if ((res = parseASTOperation12((ASTOperation *) &tempBuf, tok + n, scope))) {
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

	if ((res = parseASTOperation12((ASTOperation *) &tempBuf, tok + n, scope))) {
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

	if ((res = parseASTOperation12((ASTOperation *) &tempBuf, tok + n, scope))) {
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

int astCondOperationChildCount(ASTCondOperation const *node) {
	return 3;
}

ASTNode *astCondOperationGetChild(ASTCondOperation *node, int index) {
	return (ASTNode *[]) {
		node->condition,
		node->trueExp,
		node->falseExp,
	}[index];
}

/*************************************************************
 * Type cast Operation
 *************************************************************/

static int _parseASTCastOperation(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope *scope)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;

	initASTOperation(node, tok);
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

	if ((res = parseASTOperation2((ASTOperation *) &tempBuf, tok + n, scope))) {
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
 * Sizeof Type Operation
 *************************************************************/

static int _parseASTSizeofTypeOperation(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTOperation);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTOperation(node, tok);
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
	} else {
		freeASTOperation(node);
		return 0;
	}

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
	return n;
}

static int _parseASTSizeofExpOperation(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTOperation);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTOperation(node, tok);
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

	if ((res = parseASTOperation2((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->rhs = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTOperation(node);
		return 0;
	}
	node->node.type = AST_SIZEOF_EXP_OPERATION;

	return n;
}

/*************************************************************
 * Operation
 *************************************************************/

static ASTNodeVTable _opVTable = {
	{
		(FreeFunc) freeASTOperation,
		(PrintFunc) printASTOperation,
	},
	(ASTChildCount) astOperationChildCount,
	(ASTGetChild) astOperationGetChild,
};

void initASTOperation(ASTOperation *node, Token const *tok) {
	initASTNode(&node->node, tok, &_opVTable);
	node->tokType = TT_UNKNOWN;
	node->lhs = NULL;
	node->rhs = NULL;
}

void freeASTOperation(ASTOperation *node) {
	if (node->lhs) {
		freeASTNode(node->lhs);
		free(node->lhs);
		node->lhs = NULL;
	}
	if (node->rhs) {
		freeASTNode(node->rhs);
		free(node->rhs);
		node->rhs = NULL;
	}
	node->node.type = AST_UNKNOWN;
}

int parseASTOperation(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope *scope)
{
	return parseASTOperation15(node, tok, scope);
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
	(ASTNode *, const Token *, ASTScope *scope);
int _parseASTOperationBin(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope,
		TokenType types[],
		_ParseOperationFunc lhsFunc,
		_ParseOperationFunc rhsFunc)
{
	int res, n = 0;
	ASTNodeBuf tempBuf;
	TokenType tempType;

	initASTOperation(node, tok);
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
		node->lhs = NULL;
		freeASTNode((ASTNode *) node);
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
		ASTScope *scope,
		TokenType types[],
		_ParseOperationFunc rhsFunc)
{
	AST_VALID(ASTOperation);
	int res, n = 0;
	ASTNodeBuf tempBuf;
	TokenType tempType;

	initASTOperation(node, tok);
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

/** 
 * @brief Parses `,` operations
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation15(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation15Types,
			(_ParseOperationFunc) parseASTOperation14,
			(_ParseOperationFunc) parseASTOperation15);
}

/**
 * @brief Parses operations with precedence 14
 * @param[out] node
 * @param[in] tok
 * @parma[in] scope
 * @returns The number of tokens parsed
 *
 * Parses the following operators:
 *
 * `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `<<=`, `>>=`, `&=`, `^=`, `|=`
 */
int parseASTOperation14(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation14Types,
			(_ParseOperationFunc) parseASTOperation13,
			(_ParseOperationFunc) parseASTOperation14);
}

/**
 * @brief Parses ternary conditions
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation13(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	int n = 0, res;
	ASTCondOperation condOperation;

	initASTOperation(node, tok);
	if (astHasErr()) {
		freeASTOperation(node);
		return 0;
	}

	if ((res = parseASTCondOperation((ASTCondOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTOperation12((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else {
		freeASTOperation(node);
		return 0;
	}

	return n;
}

/**
 * @brief Parses the `||` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation12(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node, 
			tok, 
			scope,
			operation12Types, 
			(_ParseOperationFunc) parseASTOperation11,
			(_ParseOperationFunc) parseASTOperation12);
}

/**
 * @brief Parses the `&&` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation11(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation11Types,
			(_ParseOperationFunc) parseASTOperation10,
			(_ParseOperationFunc) parseASTOperation11);
}

/**
 * @brief Parses the `|` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation10(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok, 
			scope,
			operation10Types, 
			(_ParseOperationFunc) parseASTOperation9,
			(_ParseOperationFunc) parseASTOperation10);
}

/**
 * @brief Parses the `^` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation9(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation9Types,
			(_ParseOperationFunc) parseASTOperation8,
			(_ParseOperationFunc) parseASTOperation9);
}

/**
 * @brief Parses the `&` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation8(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation8Types,
			(_ParseOperationFunc) parseASTOperation7,
			(_ParseOperationFunc) parseASTOperation8);
}

/**
 * @brief Parses the `==`, `!=` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation7(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation7Types,
			(_ParseOperationFunc) parseASTOperation6,
			(_ParseOperationFunc) parseASTOperation7);
}

/**
 * @brief Parses the `<`, `<=`, `>`, `>=` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation6(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation6Types,
			(_ParseOperationFunc) parseASTOperation5,
			(_ParseOperationFunc) parseASTOperation6);
}

/**
 * @brief Parses the `<<`, `>>` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation5(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation5Types,
			(_ParseOperationFunc) parseASTOperation4,
			(_ParseOperationFunc) parseASTOperation5);
}

/**
 * @brief Parses the `+`, `-` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation4(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation4Types,
			(_ParseOperationFunc) parseASTOperation3,
			(_ParseOperationFunc) parseASTOperation4);
}

/**
 * @brief Parses the `*`, `/`, `%` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation3(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	return _parseASTOperationBin(
			node,
			tok,
			scope,
			operation3Types,
			(_ParseOperationFunc) parseASTOperation2,
			(_ParseOperationFunc) parseASTOperation3);
}

/**
 * @brief Parses prefix operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 *
 * Parses the following operators:
 *
 * `++`, `--`, `+`, `-`, `!`, `~`, typecast, `*`, `&`, sizeof
 */
int parseASTOperation2(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope)
{
	int n = 0, res;

	if ((res = _parseASTOperationPref(
		node,
		tok + n,
		scope,
		operation2Types,
		(_ParseOperationFunc) parseASTOperation2)))
	{
		n += res;
	} else if ((res = _parseASTCastOperation(node, tok + n, scope))) {
		n += res;
	} else if ((res = _parseASTSizeofTypeOperation(node, tok + n, scope))) {
		n += res;
	} else if ((res = _parseASTSizeofExpOperation(node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTOperation1(node, tok + n, scope))) {
		n += res;
	} else {
		return 0;
	}

	return n;
}

int _parseASTOperation1Chained(
		ASTOperation *node,
		const Token *tok,
		ASTScope *scope,
		ASTNode *lhs)
{
	int n = 0, res;

	initASTOperation(node, tok);
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
		if ((res = _parseASTExpSing((ASTNode *) &tempBuf, tok + n, scope))) {
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

/**
 * @brief Parses postfix operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @param[in, opt] 
 * @returns The number of tokens parsed
 *
 * Parses the following operators:
 *
 * `++`, `--`, `()`, `[]`, `.`, `->`
 */
int parseASTOperation1(ASTOperation *node, Token const *tok, ASTScope *scope) {
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTNode((ASTNode *) node, tok, NULL);
	if (astHasErr()) {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	if ((res = _parseASTExpSing((ASTNode *) node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	while (1) {
		if ((res = _parseASTOperation1Chained((ASTOperation *) &tempBuf, tok + n, scope, (ASTNode *) node))) {
			mvASTNode((ASTNode *) node, (ASTNode *) &tempBuf);
			n += res;
		} else {
			return n;
		}
	}
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

int astOperationChildCount(ASTOperation const *node) {
	return 2;
}

ASTNode *astOperationGetChild(ASTOperation *node, int index) {
	return (ASTNode *[]) {
		node->lhs,
		node->rhs,
	}[index];
}
