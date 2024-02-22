#pragma once

#include "../ast/file.h"
#include "../util/wordList.h"
#include "scope.h"

/**
 * @file
 * @brief Generates semantic types
 */

typedef struct TypeTravCtx {
	int errorCount;
	ASTScope *scope;
	ASTNode *node;
} TypeTravCtx;

/**
 * @brief Generates semantic types in the ast
 * @param[in,out] file
 * @returns 0 on success, 1 on failure
 */
int typeGen(ASTFile *file);
