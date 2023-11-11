#pragma once

#include <stdlib.h>

#include "../util/dlist.h"
#include "../ast/declaration.h"
#include "../sem/scope.h"

/*************************************************************
 * Semantic Type
 *************************************************************/

typedef enum STypeT {
	STT_UNKNOWN,
	STT_VOID,
	STT_PRIM,
	STT_ARRAY,
	STT_STRUCT,
	STT_UNION,
	STT_FUNC,
	STT_POINTER,
	STT_TYPEDEF_REF,
} STypeT;

typedef struct SType {
	STypeT type;
	unsigned char isConst: 1;
	unsigned char isVolatile: 1;
	unsigned char isTypedef: 1;
} SType;

#define STYPE_S 64
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

void initSType(SType *type);
void destroySType(SType *type);
/**
 * @brief loads the types in the declaration into the scope
 */
int loadSTypes(ASTScope *scope, ASTDeclaration *declaration);
void mvSType(SType *dest, SType const *src);
SType *movaSType(SType const *src);
const char *sttStr(STypeT t);
int printSType(SType const *type);

/*************************************************************
 * Semantic Primitive Type
 *************************************************************/

typedef enum SPrimType {
	SPT_UNKNOWN,
	SPT_CHAR,
	SPT_UCHAR,
	SPT_SCHAR,
	SPT_SHORT,
	SPT_USHORT,
	SPT_INT,
	SPT_UINT,
	SPT_LONG,
	SPT_ULONG,
	SPT_FLOAT,
	SPT_DOUBLE,
	SPT_LDOUBLE,

} SPrimType;

typedef struct SPrim {
	SType type;
	SPrimType primType;
} SPrim;

void initSPrim(SPrim *type);
int loadSPrim(SPrim *type, ASTTypeSpec *typeSpec);
const char *sptStr(SPrimType t);
int printSPrim(SPrim const *type);

/*************************************************************
 * Semantic Array Type
 *************************************************************/

typedef struct SArray {
	SType type;
	char hasSize;
	size_t size;
	SType *elType;
} SArray;

void initSArray(SArray *type);
void destroySArray(SArray *type);
int loadSArray(SArray *type, SType *internal, ASTArrayDecl *arrayDecl, ASTScope *scope);
int printSArray(SArray const *type);

/*************************************************************
 * Semantic Compound Types
 * Used for both unions and structs
 *************************************************************/

/* The underlying struct or union type that other variable types reference */
typedef struct SCompound {
	/* reference to the scope containing members (doesn't own) */
	ASTScope *scope;
	char isUnion;
} SCompound;

void initSCompound(SCompound *type);
int loadSCompound(SCompound *type, ASTStructDecl *decl);

/* Reference to the struct type */
typedef struct SCompoundRef {
	SType type;
	/* index into structs */
	int index;
	ASTScope *parentScope;
} SCompoundRef;

void initSCompoundRef(SCompoundRef *type);
/*
 * Creates a compound ref from declaration. If it is an anonymous struct,
 * struct is added to scope
 */
int loadSCompoundRef(
		SCompoundRef *type,
		ASTStructDecl *declaration,
		ASTScope *scope);
int printSCompoundRef(SCompoundRef const *ref);

/*************************************************************
 * Semantic Pointer Type
 *************************************************************/

typedef struct SPointer {
	SType type;
	SType *internal;
} SPointer;

SPointer *newSPointer();
void initSPointer(SPointer *type);
void destroySPointer(SPointer *type);
int loadSPointer(
		SType *type,
		SType *internal,
		ASTDeclarator *declarator,
		ASTScope *scope);
int printSPointer(SPointer const *type);

/*************************************************************
 * Semantic Function Type
 *************************************************************/

typedef struct SFunction {
	SType type;
	SType *returnType;
	DList paramTypes;
} SFunction;

void initSFunction(SFunction *type);
void destroySFunction(SFunction *type);
int loadSFunction(SFunction *type, SType *internal, ASTFuncDecl *declarator);
int printSFunction(SFunction const *func);

/*************************************************************
 * Semantic Typedef Ref
 *************************************************************/

typedef struct STypedefRef {
	SType type;
	/* Does not own this */
	ASTScope *parentScope;
	int index;
} STypedefRef;

void initSTypedefRef(STypedefRef *type);
int loadSTypedefRef(STypedefRef *type, ASTIdentifier *typeSpec, ASTScope *scope);
SType *stypdefDeref(STypedefRef *ref);
int printSTypedefRef(STypedefRef const *type);
