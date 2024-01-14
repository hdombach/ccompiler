#pragma once

#include "iteration.h"
#include "expression.h"
#include "selection.h"
#include "label.h"
#include "node.h"

/**
 * @brief Tools for basic statements
 */

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

/**
 * @brief Just a semi colon
 */
typedef ASTNode ASTEmptyStm;

/**
 * @param[in] node
 * @param[in] tok
 */
void initASTEmptyStm(ASTEmptyStm *node, Token const *tok);
/**
 * @param[out] node
 * @param[int] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTEmptyStm(ASTEmptyStm *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Debug print an empty statement
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTEmptyStm(ASTEmptyStm const *node);
/**
 * @brief Impliments astChildCount for ASTEmptyStm
 * @param[in] node
 * @returns The child count
 */
int astEmptyStmChildCount(ASTEmptyStm const *node);
/**
 * @brief Impliments astGetChild for ASTEmptyStm
 * @param[in] node
 * @param[in] index
 * @returns Corresponding node
 */
ASTNode *astEmptyStmGetChild(ASTEmptyStm *node, int index);

/***********************************************************************
 * Break Statement
 ***********************************************************************/

/**
 * @brief Beak statement
 */
typedef ASTNode ASTBreak;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTBreak(ASTBreak *node, Token const *tok);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTBreak(ASTBreak *node,  Token const *tok, struct ASTScope *scope);
/**
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTBreak(ASTBreak const *node);
/**
 * @brief Impliments astChildCount for ASTBreak
 * @param[in] node
 * @returns The number of child nodes
 */
int astBreakChildCount(ASTBreak const *node);
/**
 * @brief Impliments astGetChild for ASTBreak
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astBreakGetChild(ASTBreak *node, int index);

/***********************************************************************
 * Continue Statement
 ***********************************************************************/

/**
 * @brief Continue statement
 */
typedef ASTNode ASTContinue;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTContinue(ASTContinue *node, Token const *tok);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTContinue(ASTContinue *node, Token const *tok, struct ASTScope *scope);
/**
 * @param[out] node
 * @returns The number of characters printed
 */
int printASTContinue(ASTContinue const *node);
/**
 * @brief Impliments astChildCount for ASTContinue
 * @param[in] node
 * @returns The number of children
 */
int astContinueChildCount(ASTContinue const *node);
/**
 * @brief Impliments astGetChild for ASTContinue
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astContinueGetChild(ASTContinue *node, int index);

/***********************************************************************
 * Goto Statement
 ***********************************************************************/
/**
 * @brief Goto statement
 */
typedef struct ASTGoto {
	ASTNode node;
	/** @brief The label to goto */
	char *name;
} ASTGoto;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTGoto(ASTGoto *node, struct Token const *tok);
/**
 * @param[in] node
 */
void freeASTGoto(ASTGoto *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTGoto(
		ASTGoto *node,
		struct Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print the goto statement
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTGoto(ASTGoto const *node);
/**
 * @brief Impliments astChildCount for ASTGoto
 * @param[in] node
 * @returns The number of children nodes
 */
int astGotoChildCount(ASTGoto *node);
/**
 * @brief Impliments astGetChild for ASTGoto
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astGotoGetChild(ASTGoto *node, int index);

/***********************************************************************
 * Return Statement
 ***********************************************************************/

/**
 * @brief Return statements
 */
typedef struct ASTReturn {
	ASTNode node;
	/** @brief expression value to be returned */
	ASTNode *value;
} ASTReturn;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTReturn(ASTReturn *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTReturn(ASTReturn *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTReturn(
		ASTReturn *node,
		Token const *tok,
		struct ASTScope *scope);
/**
 * @brief Debug print the return statement
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTReturn(ASTReturn const *node);
/**
 * @brief Impliments astChildCount for ASTReturn
 * @param[in] node
 * @returns The number of children nodes
 */
int astReturnChildCount(ASTReturn *node);
/**
 * @brief Impliments astGetChild for ASTReturn
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astReturnGetChild(ASTReturn *node, int index);

/***********************************************************************
 * Statement
 ***********************************************************************/

/**
 * @brief A wrapper class for any statement
 */
typedef struct ASTStm {
	ASTNode node;
	DList labels;
	ASTNode *content;
} ASTStm;

/**
 * @param[out] node
 * @param[in] tok
 */
void initASTStm(ASTStm *node, Token const *tok);
/**
 * @param[in] node
 */
void freeASTStm(ASTStm *node);
/**
 * @param[out] node
 * @param[in] tok
 * @param[in] scope
 * @returns The number of tokens parsed
 */
int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope *scope);
/**
 * @brief Debug print a statement
 * @param[in] node
 * @returns The number of characters printed
 */
int printASTStm(ASTStm const *node);
/**
 * @brief Impliments astChildCount for ASTStm
 * @param[in] node
 * @returns The number of children
 */
int astStmChildCount(ASTStm const *node);
/**
 * @brief Impliments astGetChild for ASTStm
 * @param[in] node
 * @param[in] index
 * @returns The corresponding node
 */
ASTNode *astStmGetChild(ASTStm *node, int index);
