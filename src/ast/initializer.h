#pragma once

#include "../tok/token.h"
#include "../util/dlist.h"
#include "node.h"

/**
 * @file
 *
 * @brief Tools associated with ast initializer nodes
 */

typedef enum {
	AST_IT_UNKNOWN,
	AST_IT_NODE,
	AST_IT_LIST,
} ASTInitializerType;

struct ASTScope;

/**
 * @extends ASTNode
 */
typedef struct ASTInitializerList {
	ASTNode node;
	/**
	 * @brief List of expressions inside initializer list
	 *
	 * Can contain either another ASTInitializerList or an expression with precedence
	 * 14 or lower. (Any expression besides the `,` operator)
	 */
	DList list;
} ASTInitializerList;

/**
 * @param[out] list
 * @param[in] tok
 */
void initASTInitializerList(ASTInitializerList *list, Token const *tok);
/**
 * @param[in] list
 */
void freeASTInitializerList(ASTInitializerList *list);
/**
 * @param[out] list
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTInitializerList(
		ASTInitializerList *list,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print ASTInitializerList
 * @param[in] list
 * @returns The number of characters printed
 */
int printASTInitializerList(ASTInitializerList const *list);
/**
 * @brief Impliments astNodeChildCount for ASTInitializerList
 * @param[in] node
 * @returns The number of child nodes
 */
int astInitializerListChildCount(ASTInitializerList const *node);
/**
 * @brief Impliments astNodeGetChild for ASTInitializerList
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astInitializerListGetChild(ASTInitializerList *node, int index);

/**
 * @brief Parses an item which can appear in initializer list
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 *
 * Can be an initializer list or an expression with precedence 14 or lower
 */
int parseASTInitializer(
		ASTNode *node,
		Token const *tok,
		struct ASTScope *scope);
