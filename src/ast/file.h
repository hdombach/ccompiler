#pragma once

#include "funcDef.h"
#include "node.h"
#include "../sem/scope.h"

/**
 * @file
 *
 * @brief The tools for the root node of a single compilation unit
 */


/**
 * @brief The root node for a single compilation unit
 * @extends ASTNode
 */
typedef struct {
	ASTNode node;
	/** @brief Every element in the file */
	DList items;
	/**
	 * @brief Defines the namespace for defined types 
	 *
	 * @note Is not setup during the intial generation of the abstract syntax tree.
	 * It is primarily used for later compilation stages like symbolic analysis and
	 * machine code generation
	 */
	struct ASTScope *scope;
} ASTFile;

/**
 * @brief parses declarations or function definition
 * @param[out] item
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTFileItem(ASTNode *item, Token const *tok, struct ASTScope *scope);

/**
 * @param[out] file
 * @param[in] tok
 */
void initASTFile(ASTFile *file, Token const *tok);
/**
 * @param[in] file
 */
void freeASTFile(ASTFile *file);
/**
 * @param[out] file
 * @param[in] tok
 * @returns The number of tokens parsed
 */
int parseASTFile(ASTFile *file, Token const *tok);
/**
 * @param[in] file
 * @returns The number of characters printed
 */
int printASTFile(ASTFile const *file);
/**
 * @brief Impliments astNodeChildCount for ASTFile
 * @param[in] node
 * @returns The number of children nodes
 */
int astFileChildCount(ASTFile const *node);
/**
 * @brief Impliments astNodeGetChild for ASTFile
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astFileGetChild(ASTFile *node, int index);
