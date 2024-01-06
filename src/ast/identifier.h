#pragma once

#include <string.h>
#include <stdio.h>

#include "node.h"
#include "../tok/token.h"
#include "../util/util.h"
#include "../sem/type.h"
#include "../util/log.h"

/**
 * @file
 *
 * @brief Tools associated with generic identifier
 */

struct Token;
struct ASTScope;
typedef struct STypeRef STypeRef;

/**
 * @extends ASTNode
 */
typedef struct ASTIdentifier {
	ASTNode node;
	/** @note Is owned by ASTIdentifier */
	char *name;
	/** @brief A type object populated by the semantic analysis stage*/
	STypeRef typeRef;
} ASTIdentifier;

/**
 * @param[in] node
 */
void initASTIdentifier(ASTIdentifier *node);

void freeASTIdentifier(ASTIdentifier *node);

/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTIdentifier(
		ASTIdentifier *node,
		struct Token const *tok,
		struct ASTScope *scope);

/**
 * @brief Debug print the ASTIdentifier
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTIdentifier(ASTIdentifier const *node);

/**
 * @brief Impliments astNodeChildCount for ASTIdentifier
 * @param[in] node
 * @return the number of chid nodes
 */
int astIdentifierChildCount(ASTIdentifier const *node);

/**
 * @brief Impliments astNodeGetChild for ASTIdentifier
 * @param[in] node
 * @param[in] index
 * @returns The corresponding child
 */
ASTNode *astIdentifierGetChild(ASTIdentifier *node, int index);
