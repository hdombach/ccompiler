#pragma once

#include "../util/dlist.h"
#include "../tok/token.h"
#include "node.h"
#include "statement.h"
#include "declaration.h"
#include "../sem/scope.h"

/**
 * @file
 *
 * @brief Compound Statement AST Node
 */
struct ASTScope;

/**
 * @brief Compound Statement
 * @extends ASTNode
 *
 * Functions to work with ASTCompStm are in @ref compStatement.h
 */
typedef struct ASTCompStm {
	ASTNode node;
	/** @brief The child statements */
	DList items;
	/** @brief The collection of types */
	ASTScope *scope;
} ASTCompStm;

/**
 * @brief Parses a child statement
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns Number of tokens parsed
 */
int parseASTCompItem(
		ASTNode *node,
		Token const *tok,
		struct ASTScope *scope);

/**
 * @param[out] node
 * @param[in] tok Starting token
 */
void initASTCompStm(ASTCompStm *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTCompStm(ASTCompStm *node);
/**
 * @brief Parses the compound statement from token list
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns Number of tokens parsed
 */
int parseASTCompStm(
		ASTCompStm *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print compound statement
 * @param[in] node
 * @returns Number of tokens parsed
 */
int printASTCompStm(ASTCompStm const *node);
/**
 * @brief Impliments astNodeChildCount for ASTCompStm
 * @param[in] node
 * @returns Number of children in tree
 */
int astCompStmChildCount(ASTCompStm const *node);
/**
 * @brief Impliments astNodeGetChild for ASTCompStm
 * @param[in] node
 * @param[in] index
 * @returns The node at the corresponding index
 */
ASTNode *astCompStmGetChild(ASTCompStm *node, int index);
