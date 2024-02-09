#pragma once

/**
 * @file
 * 
 * @brief Tools for function semantic type
 */

#include "type.h"

/**
 * @extends SType
 */
typedef struct SFunction {
	SType type;
	SType *returnType;
	ASTScope *paramScope;
} SFunction;

/**
 * @param[out] type
 */
void initSFunction(SFunction *type);
/**
 * @param[in] type
 */
void destroySFunction(SFunction *type);
/**
 * @param[out] type
 * @param[in] internal
 * @param[in] declarator
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadSFunction(
		SType *type,
		SType *internal,
		ASTFuncDecl *declarator,
		ASTScope *scope);
/**
 * @brief Debug print the SFunction
 * @param[in] func
 * @returns The number of characters printed
 */
int printSFunction(SFunction const *func);

/*************************************************************
 * Semantic Typedef Ref
 *************************************************************/


