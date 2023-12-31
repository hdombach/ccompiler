#pragma once

#include "../token.h"
#include "node.h"

/**
 * @file
 *
 * Tools for parsing int constants
 */

/**
 * @brief Represents an int constant
 * @extends ASTNode
 *
 * Internally holds an unsigned int becuase signed numbers are handeled
 * later through the `-` operation
 * 
 * ASTIntConstant does not need to be initialized or freed
 * becuase it does not own any buffers
 */
typedef struct {
	ASTNode node;
	/** @brief The parsed int */
	unsigned long int value;
} ASTIntConstant;

/**
 * @brief Parses the int constant statement from the token list
 * @param[out] node
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTIntConstant(ASTIntConstant *node, Token const *tok);

/**
 * @brief Debug print the int constant
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTIntContant(ASTIntConstant const *node);

/**
 * @brief Impliments astNodeChildCount for ASTIntConstant
 * @param[in] node
 * @returns Number of children in tree
 */
int astIntConstantChildCount(ASTIntConstant const *node);
/**
 * @brief Impliments astNodeGetChild for ASTIntConstant
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astIntConstantGetChild(ASTIntConstant *node, int index);
