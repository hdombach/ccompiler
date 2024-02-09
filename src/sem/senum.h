#pragma once

/**
 * @file
 *
 * @brief Tools for enum semantic type
 */

#include "type.h"

/**
 * @extends SType
 */
typedef struct SEnum {
	SType type;
	ASTScope *scope;
} SEnum;
/**
 * @param[out] type
 */
void initSEnum(SEnum *type);
/**
 * @param[in] type
 * @returns The number of characters printed
 */
int printSEnum(SEnum *type);
/**
 * @param[out] type
 * @param[in] decl
 * @returns Whether it succeeds
 */
int loadSEnum(SEnum *type, ASTEnumDecl *decl);

/**
 * A reference to an enum
 */
typedef struct SEnumRef {
	int index;
	ASTScope *parentScope;
} SEnumRef;

/**
 * @param[out] type
 */
void initSEnumRef(SEnumRef *type);
/**
 * @param[out] type
 * @param[in] declaration
 * @param[in] scope
 * @returns if it succeeds
 */
int loadSEnumRef(
		SEnumRef *type,
		ASTEnumDecl *declaration,
		ASTScope *scope);
/**
 * @brief Debug print a SEnumRef
 * @param[out] ref
 * @returns The number f characters printed.
 */
int printSEnumRef(SEnumRef const *ref);
/**
 * @brief Gets the underlying enum
 * @param[in] ref
 * @returns The underlying enum
 */
SEnum *senumDeref(SEnumRef const *ref);

/**
 * @param[out] type
 * @param[in] decl
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadSEnumConst(SPrim *type, ASTEnumConst *decl, ASTScope *scope);
