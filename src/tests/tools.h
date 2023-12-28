#pragma once

#include "../util/log.h"
#include "../ast/node.h"
#include "../token.h"
#include "../ast/node.h"

/**
 * @file
 *
 * @brief Helper test functions
 */

void tTokensSuccess(const char *code, TokenType types[]);

void tTokensFailed(const char *code, CError *errors);

/**
 * @brief Tests the generated ast against types
 * @param[in] code Source code to parse
 * @param[in] types List of types to test ast against
 *
 * @details Parses the source code up to the ast generation.
 * Then does an pre-order traverasal and matches the generated
 * node types with the given list of types
 */
void tAstSuccess(const char *code, ASTNodeType types[]);

void tAstFailed(const char *code, CError *error);

/**
 * @brief Prints out the list of ASTNodeTypes that are parsed
 * @param[in] code
 * 
 * @details Parses the code up to the ast generation. Then
 * It does a pre-order traversal and prints out the node types
 */
void tAstDebug(const char *code);
