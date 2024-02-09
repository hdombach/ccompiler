#pragma once

/**
 * @file
 *
 * @brief Tools for union and struct semantic types
 */

#include "type.h"

/**
 * @extends SType
 * @brief The underlying struct or union type that other variable types reference
 */
typedef struct SCompound {
	SType type;
	/**
	 * @brief reference to the scope containing members
	 * @note Doesn't own
	 */
	ASTScope *scope;
	char isUnion;
} SCompound;
/**
 * @param[out] type
 */
void initSCompound(SCompound *type);
/**
 * @brief Debug print SCompound
 * @param[int] ref
 * @returns The number of characters printed
 */
int printSCompound(SCompound const *ref);
/**
 * @param[out] type
 * @param[in] decl
 * @returns Whether it succeeds
 */
int loadSCompound(SCompound *type, ASTStructDecl *decl);

/**
 * @brief Reference to the struct type
 */
typedef struct SCompoundRef {
	/* index into structs */
	int index;
	ASTScope *parentScope;
} SCompoundRef;

void initSCompoundRef(SCompoundRef *type);
/*
 * @extends SType
 * @brief Creates a compound ref from declaration.
 * @note If it is an anonymous struct, struct is added to scope
 */
int loadSCompoundRef(
		SCompoundRef *type,
		ASTStructDecl *declaration,
		ASTScope *scope);
/**
 * @brief Debug print SCompoundRef
 * @param[in] ref
 * @reutnrs The number of characters printed
 */
int printSCompoundRef(SCompoundRef const *ref);
/**
 * @brief Gets the underlying SCompound from the scope
 * @param[in] ref
 * @returns The corresponding compound
 */
SCompound *scompoundDeref(struct SCompoundRef *ref);


