#pragma once

/**
 * @file
 *
 * @brief Tools for pointer semantic types
 */

#include "type.h"

/**
 * @extends SType
 */
typedef struct SPointer {
	SType type;
	SType *internal;
} SPointer;

/**
 * @returns SPointer in new buffer
 */
SPointer *newSPointer();
/**
 * @param[out] type
 */
void initSPointer(SPointer *type);
/**
 * @param[in] type
 */
void destroySPointer(SPointer *type);
/**
 * @param[out] type
 * @param[in] internal
 * @param[in] declarator
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadSPointer(
		SType *type,
		SType *internal,
		ASTDeclarator *declarator,
		ASTScope *scope);
/**
 * @param[in] type
 * @returns The number of characters printed
 */
int printSPointer(SPointer const *type);

/**
 * @brief Impliments stypeGetIntern for SPointer
 */
SType *spointerGetIntern(SPointer *type);
