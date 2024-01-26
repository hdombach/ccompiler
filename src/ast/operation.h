#pragma once

#include "../util/dlist.h"
#include "../tok/token.h"
#include "node.h"

/**
 * @file
 *
 * @brief Tools for parsing operations and other expressions
 */

struct ASTDeclaration;
struct ASTParam;
struct ASTScope;


/*************************************************************
 * Func Operation
 *************************************************************/

/**
 * @extends ASTNode
 */
typedef struct ASTFuncOperation {
	ASTNode node;
	/** @brief expression representing function to call*/
	ASTNode *func;
	DList params;
} ASTFuncOperation;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTFuncOperation(ASTFuncOperation *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTFuncOperation(ASTFuncOperation *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] func
 * @param[in] scope
 * @note If the parse fails, func is not freed
 */
int parseASTFuncOperation(
		ASTFuncOperation *node,
		Token const *tok,
		ASTNode *func,
		struct ASTScope *scope);
/**
 * @brief Debug print for ASTFuncOperation
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTFuncOperation(ASTFuncOperation const *node);
/**
 * @brief Impliments astNodeChildCount for ASTFuncOperation
 * @param[in] node
 * @returns The number of child nodes
 */
int astFuncOperationChildCount(ASTFuncOperation const *node);
/**
 * @brief Impliments astNodeGetChild for ASTFuncOperation
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astFuncOperationGetChild(ASTFuncOperation *node, int index);

/*************************************************************
 * Cond Operation
 *************************************************************/

/**
 * @brief A ternerary operation
 * @extends ASTNode
 */
typedef struct ASTCondOperation {
	ASTNode node;
	ASTNode *condition;
	ASTNode *trueExp;
	ASTNode *falseExp;
} ASTCondOperation;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTCondOperation(ASTCondOperation *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTCondOperation(ASTCondOperation *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTCondOperation(
		ASTCondOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print for ASTCondOperation
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTCondOperation(ASTCondOperation const *node);
/**
 * @brief Impliments astNodeChildCount for ASTCondOperation
 * @param[in] node
 * @returns The number of child nodes
 */
int astCondOperationChildCount(ASTCondOperation const *node);
/**
 * @brief Impliments astNodeGetChild for ASTCondOperation
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astCondOperationGetChild(ASTCondOperation *node, int index);

/*************************************************************
 * Operation
 *************************************************************/

/*
 * @brief Is used for Subscript, cast, sizeof, binary, prefix, postfix
 * @extends ASTNode
 */
typedef struct ASTOperation {
	ASTNode node;
	TokenType tokType;
	ASTNode *lhs;
	ASTNode *rhs;
} ASTOperation;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTOperation(ASTOperation *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTOperation(ASTOperation *node);
/**
 * @brief Primary method of parsing operation / expression
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `,` operation
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation15(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses operations with precedence 14
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 *
 * Parses the following operators
 *
 * `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `<<=`, `>>=`, `&=`, `^=`, `|=`
 */
int parseASTOperation14(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses ternerary conditions
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation13(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `||` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns the number of tokens parsed
 */
int parseASTOperation12(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `&&` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation11(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `|` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation10(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `^` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation9(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Parses the `&` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTOperation8(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
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
		Token const *tok,
		struct ASTScope *scope);
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
		Token const *tok,
		struct ASTScope *scope);
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
		Token const *tok,
		struct ASTScope *scope);

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
		Token const *tok,
		struct ASTScope *scope);
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
		Token const *tok,
		struct ASTScope *scope);
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
		Token const *tok,
		struct ASTScope *scope);
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
int parseASTOperation1(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope *scope);
int printASTOperation(ASTOperation const *node);

int astOperationChildCount(ASTOperation const *node);
ASTNode *astOperationGetChild(ASTOperation *node, int index);
