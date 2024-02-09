#pragma once

#include "type.h"

/**
 * @file
 *
 * @brief Semantic type for primitives
 */

/**
 * @brief A [x macro](https://en.wikipedia.org/wiki/X_macro) for CError
 */
#define X_PRIM_TYPE \
	X(SPT_UNKNOWN, "(unknown)") \
	X(SPT_CHAR, "char") \
	X(SPT_UCHAR, "unsigned char") \
	X(SPT_SCHAR, "signed char") \
	X(SPT_SHORT, "short") \
	X(SPT_USHORT, "unsigned short") \
	X(SPT_INT, "int") \
	X(SPT_UINT, "unsigned int") \
	X(SPT_LONG, "long") \
	X(SPT_ULONG, "unsigned long") \
	X(SPT_FLOAT, "float") \
	X(SPT_DOUBLE, "double") \
	X(SPT_LDOUBLE, "long double") \

#define X(NAME, STR) NAME,
/**
 * @brief Semantic primitive type
 */
typedef enum SPrimType {
	X_PRIM_TYPE
} SPrimType;
#undef X

/**
 * @extends SType
 * @brief Used for primitive types and enumeration constants
 */
typedef struct SPrim {
	SType type;
	SPrimType primType;
} SPrim;

/**
 * @param[out] type
 */
void initSPrim(SPrim *type);
/**
 * @param[out] type
 * @param[int] typeSpec
 * @returns Whether it succeeds
 */
int loadSPrim(SPrim *type, ASTTypeSpec *typeSpec);
/**
 * @brief String repsentation of SPrimType
 * @param[in] t
 * @returns String representation
 */
const char *sptStr(SPrimType t);
/**
 * @brief Debug print a SPrim
 * @param[in] type
 * @returns The number of characters printed
 */
int printSPrim(SPrim const *type);


