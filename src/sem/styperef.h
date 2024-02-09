#pragma once

/**
 * @file
 *
 * @brief Tools for typeref semantic type
 */

#include "type.h"

/**
 * @extends SType
 * @note A typedef that references another type
 */
typedef struct STypeRef {
	SType type;
	/* Does not own this */
	ASTScope *parentScope;
	int index;
} STypeRef;

/**
 * @param[out] type
 */
void initSTypeRef(STypeRef *type);
/**
 * @param[out] type
 * @param[in] typeSpec
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadSTypeRef(STypeRef *type, ASTIdentifier *typeSpec, ASTScope *scope);
/**
 * @brief Gets the underlying stype from scope
 * @param[in] ref
 * @returns The underlying type
 */
SType *stypeDeref(STypeRef *ref);
/**
 * @brief Debug print the STypeRef
 * @param[in] type
 * @returns The number of characters printed
 */
int printSTypeRef(STypeRef const *type);

/**
 * @param[in] lhs
 * @param[in] rhs
 * @returns Whether they are compatible
 */
int stypeCompatible(SType *lhs, SType *rhs);


