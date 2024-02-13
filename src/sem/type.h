#pragma once

/**
 * @file
 *
 * @brief Semantic types
 */

//#include "../ast/declaration.h"
#include "../sem/scope.h"
#include "../ast/param.h"

typedef struct ASTIdentifier ASTIdentifier;
typedef struct ASTDeclaration ASTDeclaration;
typedef struct SType SType;
typedef struct SPrim SPrim;

/*************************************************************
 * Semantic Type
 *************************************************************/

/**
 * @brief A [x macro](https://en.wikipedia.org/wiki/X_macro) for CError
 */
#define X_STYPE_TYPE \
	X(STT_UNKNOWN, "(unknown)") \
	X(STT_VOID, "void") \
	X(STT_PRIM, "primitive") \
	X(STT_ARRAY, "array") \
	X(STT_STRUCT, "struct") \
	X(STT_UNION, "union") \
	X(STT_STRUCT_REF, "struct ref") \
	X(STT_UNION_REF, "union ref") \
	X(STT_FUNC, "function") \
	X(STT_POINTER, "pointer") \
	X(STT_TYPEDEF_REF, "typedef ref") \
	X(STT_ENUM, "enum") \
	X(STT_ENUM_REF, "enum ref") \
	X(STT_ENUM_CONST, "enum const") \

#define X(NAME, STR) NAME,
/**
 * @brief Semantic type type
 */
typedef enum STypeT {
	X_STYPE_TYPE
} STypeT;
#undef X

/**
 * @extends VTable
 */
typedef struct STypeVTable {
	VTable table;
} STypeVTable;

/**
 * @brief A generic semantic type
 */
typedef struct SType {
	STypeT type;
	STypeVTable const *vtable;
	unsigned char isConst: 1;
	unsigned char isVolatile: 1;
	unsigned char isTypedef: 1;
} SType;

#define STYPE_S 64
/**
 * @brief Struct for allocating a semantic type
 */
typedef struct STypeBuf {
	SType type;
	char buf[STYPE_S - sizeof(SType)];
} STypeBuf;

#if __STDC_VERSION__ >= 201112L
#define STYPE_VALID(Type)\
	static_assert(sizeof(STypeBuf) >= sizeof(Type), "SType is too small")
#else
#define STYPE_VALID(Type)
#endif

/**
 * @param[out] type
 */
void initSType(SType *type, STypeVTable *vtable);
/**
 * @param[in] type
 */
void destroySType(SType *type);
/**
 * @param[out] type
 * @param[in] internal
 * @param[in] node
 * @param[in] scope
 * @returns Whether it succeeds
 */
int loadDecl(
		SType *type,
		SType *internal,
		ASTNode *node,
		ASTScope *scope);
/**
 * @brief loads the types in the declaration into the scope
 * @param[in] scope
 * @param[in] declaration
 * @returns if success
 */
int loadSTypes(ASTScope *scope, ASTDeclaration *declaration);
/**
 * @brief loads the types in the param into the scope
 * @param[in] scope
 * @param[in] param
 * @returns success
 */
int loadParamSType(ASTScope *scope, ASTParam *param);
/**
 * @brief Basically just memcpy
 * @param[out] dest
 * @param[in] src
 */
void mvSType(SType *dest, SType const *src);
/**
 * @brief Memcpy to new buffer
 * @param[in] src
 * @returns The created buffer
 */
SType *movaSType(SType const *src);
/**
 * @brief String representation of STypeT
 * @param[in] t
 * @returns The string representation
 */
const char *sttStr(STypeT t);
/**
 * @brief Debug print a type
 * @returns The number of characters printed
 */
int printSType(SType const *type);
