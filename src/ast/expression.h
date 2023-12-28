#pragma once

#include "intConstant.h"
#include "operation.h"
#include "identifier.h"
#include "node.h"

/**
 * @file
 *
 * @brief Tools for parsing expressions
 */

struct ASTScope;

/**
 * @brief Primary function for parsing expression
 * @param[out] node
 * @param[in] toke
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses constants or identifiers
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExpSing(ASTNode *node, Token const *tok, struct ASTScope *scope);
/** 
 * @brief Parses `,` operations
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp15(ASTNode *node, Token const *tok, struct ASTScope *scope);
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
int parseASTExp14(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses ternary conditions
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp13(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `||` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp12(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `&&` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp11(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `|` operators
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp10(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `^` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp9(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `&` operator
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp8(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `==`, `!=` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp7(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `<`, `<=`, `>`, `>=` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp6(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `<<`, `>>` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp5(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `+`, `-` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp4(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses the `*`, `/`, `%` operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTExp3(ASTNode *node, Token const *tok, struct ASTScope *scope);
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
int parseASTExp2(ASTNode *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Parses postfix operators
 * @param[out] node
 * @param[in] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 *
 * Parses the following operators:
 *
 * `++`, `--`, `()`, `[]`, `.`, `->`
 */
int parseASTExp1(ASTNode *node, Token const *tok, struct ASTScope *scope);
