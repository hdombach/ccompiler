#pragma once

#include "node.h"

typedef struct Token Token;

/**
 * @file
 *
 * Tools for parsing string constants
 */

/**
 * @extends ASTNode
 */
typedef struct ASTStringConstant {
	ASTNode node;
	char *value;
} ASTStringConstant;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTStringConstant(ASTStringConstant *node, const Token *tok);

/**
 * @param[in] node
 */
void freeASTStringConstant(ASTStringConstant *node);

/**
 * @brief Parses the string constant statement from the token list
 * @param[out] node
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTStringConstant(ASTStringConstant *node, Token const *tok);

/**
 * @brief Debug print the string constant
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTStringConstant(ASTStringConstant const *node);

/**
 * @brief Impliments astNodeChildCount for ASTStringConstant
 * @param[in] node
 * @returns Number of children in tree
 */
int astStringConstantChildCount(ASTStringConstant const *node);

/**
 * @brief Impliments astNodeGetChild for ASTStringConstant
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astStringConstantGetChild(ASTStringConstant *node, int index);
