#pragma once

#include "node.h"

/**
 * @brief Labels
 */

typedef struct Token Token;
struct ASTScope;

/**
 * @brief Parses any type of label
 * @param[out] label
 * @param[in] tok
 * @param[in] scope
 */
int parseASTLbl(
		struct ASTNode *label,
		struct Token const *tok,
		struct ASTScope *scope);

/**
 * @extends ASTNode
 */
typedef struct ASTLblIdentifier {
	ASTNode node;
	char *name;
	ASTNode *stm;
} ASTLblIdentifier;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTLblIdentifier(ASTLblIdentifier *node, struct Token const *tok);
/**
 * @param[in] node
 */
void freeASTLblIdentifier(ASTLblIdentifier *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns Number of tokens parsed
 */
int parseASTLblIdentifier(
		ASTLblIdentifier *node,
		struct Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print ASTLblIdentifier
 * @paran[in] node
 * @returns Number of characters printed
 */
int printASTLblIdentifier(ASTLblIdentifier const *node);
/**
 * @brief Impliments astChildCount for ASTLblIdentifier
 * @param[in] node
 * @returns Child count
 */
int astLblIdentifierChildCount(const ASTLblIdentifier *node);
/**
 * @brief Impliments astGetChild for ASTLblIdentifier
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astLblIdentifierGetChild(ASTLblIdentifier *node, int index);

/**
 * @extends ASTNode
 */
typedef struct ASTLblCase {
	ASTNode node;
	struct ASTNode *expression;
	ASTNode *stm;
} ASTLblCase;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTLblCase(ASTLblCase *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTLblCase(ASTLblCase *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTLblCase(
		ASTLblCase *node,
		struct Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print of ASTLblCase
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTLblCase(ASTLblCase const *node);
/**
 * @brief Impliments astChildCount for ASTLblCase
 * @param[in] node
 * @returns The number of children nodes
 */
int astLblCaseChildCount(ASTLblCase const *node);
/**
 * @brief Impliments astGetChild for ASTLblCase
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astLblCaseGetChild(ASTLblCase *node, int index);

/**
 * @extends ASTNode
 */
typedef struct ASTLblDefault {
	ASTNode node;
	ASTNode *stm;
} ASTLblDefault;
/**
 * @param[out] node
 * @param[in] tok
 */
void initASTLblDefault(ASTLblDefault *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTLblDefault(ASTLblDefault *node, Token const *tok);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTLblDefault(
		ASTLblDefault *node,
		struct Token const *tok,
		struct ASTScope *scope);
/**
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTLblDefault(ASTLblDefault const *node);
/**
 * @brief Impliments astChildCount for ASTLblDefault
 * @param[in] node
 * @returns The number of children
 */
int astLblDefaultChildCount(ASTLblDefault const *node);
/**
 * @brief Impliments astGetChild for ASTLblDefault
 */
ASTNode *astLblDefaultGetChild(ASTLblDefault *node, int index);
