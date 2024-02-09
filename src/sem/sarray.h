#pragma once

/**
 * @file
 *
 * @brief Tools for array semantic type
 */

#include "type.h"

/**
 * @extends SType
 * @brief An array type
 */
typedef struct SArray {
	SType type;
	char hasSize;
	size_t size;
	SType *elType;
} SArray;
/**
 * @param[out] type
 */
void initSArray(SArray *type);
/**
 * @param[in] type
 */
void destroySArray(SArray *type);
/**
 * @param[out] type
 * @param[in] internal
 * @param[in] arrayDecl
 * @param[in] scope
 */
int loadSArray(SArray *type, SType *internal, ASTArrayDecl *arrayDecl, ASTScope *scope);
/**
 * @brief Debug print SArray
 * @param[in] type
 * @returns The number of characters printed
 */
int printSArray(SArray const *type);
