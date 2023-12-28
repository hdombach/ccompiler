#pragma once

#include <string.h>
#include <stdio.h>

#include "node.h"
#include "../token.h"
#include "../util/util.h"
#include "../sem/type.h"

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
static void initASTIdentifier(ASTIdentifier *node) {
	node->name = NULL;
	node->node.type = AST_IDENTIFIER;
	initSTypeRef(&node->typeRef);
}

/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
static int parseASTIdentifier(
		ASTIdentifier *node,
		struct Token const *tok,
		struct ASTScope *scope) 
{
	AST_VALID(ASTIdentifier);
	if (tok->type == TT_IDENTIFIER) {
		node->name = strdup(tok->contents);
		node->node.type = AST_IDENTIFIER;
		return 1;
	} else return 0;
}

/**
 * @brief Debug print the ASTIdentifier
 * @param[in] node
 * @returns The number of characters printed
 */
static int printASTIdentifier(ASTIdentifier const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	n += printf(", \"name\": ");
	n += printJsonStr(node->name);

	n += printf("}");
	
	return n;
}

/**
 * @brief Impliments astNodeChildCount for ASTIdentifier
 * @param[in] node
 * @return the number of chid nodes
 */
static int astIdentifierChildCount(ASTIdentifier const *node) {
	return 0;
}

/**
 * @brief Impliments astNodeGetChild for ASTIdentifier
 * @param[in] node
 * @param[in] index
 * @returns The corresponding child
 */
static ASTNode *astIdentifierGetChild(ASTIdentifier *node, int index) {
	return NULL;
}
