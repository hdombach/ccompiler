#pragma once

#include "../tok/token.h"
#include "node.h"

/**
 * @file
 *
 * Tools for parsing character constants
 */

/**
 * @extends ASTNode
 */
typedef struct ASTCharConstant {
	ASTNode node;
	/** @brief The parsed character */
	char value;
} ASTCharConstant;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTCharConstant(ASTCharConstant *node, const Token *tok);

/**
 * @brief Parses the character constant statement from the token list
 * @param[out] node
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTCharConstant(ASTCharConstant *node, Token const *tok);

/**
 * @brief Debug print the char constant
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTCharConstant(ASTCharConstant const *node);

/**
 * @brief Impliments astNodeChildCount for ASTCharConstant
 * @param[in] node
 * @returns Number of children in tree
 */
int astCharConstantChildCount(ASTCharConstant const *node);

/**
 * @brief Impliments astNodeGetChild for ASTCharConstant
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astCharConstantGetChild(ASTCharConstant *node, int index);
