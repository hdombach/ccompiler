#pragma once

#include "node.h"

typedef struct Token Token;

/**
 * @file
 *
 * Tools for parsing float constants
 */

/**
 * @brief What ASTFloatConstant stores internally
 *
 * Also used to determain the default numeric type for a
 * float constant
 * - `5.2`: `double`
 * - `5.2l`: `double`
 * - `5.2f`: `float`
 */
typedef enum ASTFloatConstantType {
	FCT_DOUBLE,
	FCT_FLOAT,
} ASTFloatConstantType;

/**
 * @brief Represents a float constant
 * @extends ASTNode
 *
 * Internally holds a double value for both float and doubles.
 */
typedef struct ASTFloatConstant {
	ASTNode node;
	/** @brief The parsed floating point value */
	ASTFloatConstantType valueType;
	union {
		double d;
		float f;
	} value;
} ASTFloatConstant;

/**
 * @param[in] node
 */
void initASTFloatConstant(ASTFloatConstant *node, Token const *tok);

/**
 * @brief Parses the float constant statement from the token list
 * @param[out] node
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTFloatConstant(ASTFloatConstant *node, Token const *tok);

/**
 * @brief Debug print the float constant
 * @param[in] node
 * @returns The number of tokens parsed
 */
int printASTFloatConstant(ASTFloatConstant const *node);

/**
 * @brief Impliments astNodeChildCount for ASTFloatConstant
 * @param[in] node
 * @returns Number of children in tree
 */
int astFloatConstantChildCount(ASTFloatConstant const *node);

/**
 * @brief Impliments astNodeGetChild for ASTFloatConstant
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astFloatConstantGetChild(ASTFloatConstant *node, int index);
