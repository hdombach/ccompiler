#pragma once

#include "../tok/token.h"
#include "compStatement.h"
#include "declaration.h"
#include "specialDecl.h"
#include "node.h"

/**
 * @file
 *
 * @brief The tools for function definitions
 */

struct ASTDeclarator;
struct ASTScope;

/**
 * @extends ASTNode
 */
typedef struct ASTFuncDef {
	ASTNode node;
	/** @brief The first part of the return type */
	ASTTypeSpec *typeSpec;
	/** @brief The second part of the return types and params */
	ASTNode *funcDecl;
	/** @brief The child statement nodes */
	ASTCompStm *compoundStm;
} ASTFuncDef;

/**
 * @param[out] def
 * @param[in] tok
 */
void initASTFuncDef(ASTFuncDef *def, Token const *tok);
/**
 * @param[in] def
 */
void freeASTFuncDef(ASTFuncDef *def);
/**
 * @param[out] def
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTFuncDef(
		ASTFuncDef *def,
		Token const * tok,
		struct ASTScope *scope);
/**
 * @brief Debug print for ASTFuncDef
 * @param[in] def
 * @returns The number of characters printed
 */
int printASTFuncDef(ASTFuncDef const *def);
/**
 * @brief Impliments astNodeChildCount for ASTFuncDef
 * @param[in] node
 * @returns The number of child nodes
 */
int astFuncDefChildCount(ASTFuncDef const *node);
/**
 * @brief Impliments astNodeGetChild for ASTFuncDef
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astFuncDefGetChild(ASTFuncDef *node, int index);
