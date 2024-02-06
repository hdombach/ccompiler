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
	X(STT_STRUCT_REF, "struct ref") \
	X(STT_UNION, "union") \
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

typedef SType* (*STypeDeref)(SType *type);

/**
 * @extends VTable
 */
typedef struct STypeVTable {
	VTable table;
	STypeDeref deref;
} STypeVTable;

/**
 * @brief A generic semantic type
 */
typedef struct SType {
	STypeT type;
	STypeVTable vtable;
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
void initSType(SType *type);
/**
 * @param[in] type
 */
void destroySType(SType *type);
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

/*************************************************************
 * Semantic Primitive Type
 *************************************************************/


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

/*************************************************************
 * Semantic Array Type
 *************************************************************/

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

/*************************************************************
 * Semantic Compound Types
 *************************************************************/

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

/*************************************************************
 * Semantic Pointer Type
 *************************************************************/

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

/*************************************************************
 * Semantic Function Type
 *************************************************************/

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

/*************************************************************
 * Semantic Enum
 *************************************************************/

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
