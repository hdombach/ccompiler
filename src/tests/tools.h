#pragma once

#include "../util/log.h"
#include "../ast/node.h"
#include "../tok/token.h"
#include "../ast/node.h"
#include "../sem/type.h"

/**
 * @file
 *
 * @brief Helper test functions
 */

void tTokensSuccess(const char *code, TokenType types[]);

void tTokensFailed(const char *code, CError *errors);
/**
 * @brief Prints out the list of Tokens that are parsed
 * @param[in] code
 */
void tTokensDebug(const char *code);

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

/**
 * @brief Tests the labels in generated scopes
 * @param[in] code to parse
 * @param[in] labels to expect
 * @note Sometimes, duplicated labels in list won't be tested correctly for
 */
void tTypeGenLabels(const char *code, const char **labels);

/**
 * @brief Tests if typegen fails
 * @param[in] code t parse
 * @param[in] errors to expect
 */
void tTypeGenFailed(const char *code, CError *error);

/**
 * @param[in] code
 * @param[in] types
 */
void tTypeGenSuccess(const char *code, STypeT *types);

void tTypeGenDebug(const char *code);
