#pragma once

#include "util/dlist.h"

/**
 * @file 
 * @brief Tools for preprocessor stage
 */

/**
 * @brief Applies preprocessor stage to tokens
 * @param[in,out] tokens
 *
 * Right now the preprocessor assumes that it can find the files required by
 * include statements in the current working directory
 */
void preprocessor(DList *tokens);
