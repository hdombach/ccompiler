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
typedef struct STypedef {
	SType type;
	/* Does not own this */
	ASTScope *parentScope;
	int index;
} STypedef;

/**
 * @param[out] type
 */
void initSTypedef(STypedef *type);
/**
 * @param[out] type
 * @param[in] typeSpec
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadSTypedef(STypedef *type, ASTIdentifier *typeSpec, ASTScope *scope);
/**
 * @brief Gets the underlying stype from scope
 * @param[in] ref
 * @returns The underlying type
 */
SType *stypedefDeref(STypedef *ref);
/**
 * @brief Debug print the STypedef
 * @param[in] type
 * @returns The number of characters printed
 */
int printSTypedef(STypedef const *type);

/**
 * @param[in] lhs
 * @param[in] rhs
 * @returns Whether they are compatible
 */
int stypeCompatible(SType *lhs, SType *rhs);


