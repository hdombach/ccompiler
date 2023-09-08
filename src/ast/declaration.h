#pragma once

#include "../token.h"
#include "../util/dlist.h"

// https://en.cppreference.com/w/c/language/declarations

typedef enum  {
	AST_TQ_NONE = 0b0,
	AST_TQ_CONST = 0b1,
	AST_TQ_VOLATILE = 0b10,
} ASTTypeQualifier;

typedef enum {
	AST_SC_NONE = 0b0,
	AST_SC_TYPEDEF = 0b1,
	AST_SC_AUTO = 0b10,
	AST_SC_REGISTER = 0b100,
	AST_SC_STATIC = 0b1000,
	AST_SC_EXTERN = 0b10000,
} ASTStorageClassSpec;

typedef enum {
	AST_AT_NONE = 0b0,
	AST_AT_CHAR = 0b1,
	AST_AT_INT = 0b10,
	AST_AT_SIGNED = 0b100,
	AST_AT_UNSIGNED = 0b1000,
	AST_AT_SHORT = 0b10000,
	AST_AT_LONG = 0b100000,
} ASTArithType;

//TODO: implimented enums, structs, and unions

typedef enum {
	AST_TST_UNKNOWN,
	AST_TST_VOID,
	AST_TST_ARITH,
	AST_TST_TYPEDEF,
	AST_TST_STRUCT,
	AST_TST_UNION,
	AST_TST_ENUM,
} ASTTypeSpecType;

/*
 * A type specifier
 * (The first chunk of a decleration)
 */
typedef struct {
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	ASTTypeSpecType typeSpecType;
	union {
		ASTArithType arith;
		char *typedefName;
	} c;
} ASTTypeSpec;

//TODO: add other declerator types
//TODO: add initialization thing
typedef enum {
	AST_DT_UNKNOWN,
	AST_DT_IDENTIFIER,
} ASTDeclaratorType;

typedef struct {
	ASTDeclaratorType type;
	union {
		char *identifier;
	} c;
} ASTDeclarator;

typedef struct {
	ASTTypeSpec typeSpec;
	DList declarators;
} ASTDeclaration;

void initASTTypeQualifier(ASTTypeQualifier *qualifiers);
int parseASTTypeQualifier(ASTTypeQualifier *qualifiers, Token const *tok);
int printASTTypeQualifier(ASTTypeQualifier const *qualifiers);

void initASTStorageClassSpec(ASTStorageClassSpec *specs);
int parseASTStorageClassSpec(ASTStorageClassSpec *specs, Token const *tok);
int printASTStorageClassSpec(ASTStorageClassSpec const *specs);

void initASTArithType(ASTArithType *type);
int parseASTArithType(ASTArithType *type, Token const *tok);
int printASTArithType(ASTArithType const *type);

void initASTTypeSpec(ASTTypeSpec *typeSpec);
void freeASTTypeSpec(ASTTypeSpec *typeSpec);
int parseASTTypeSpec(ASTTypeSpec *typeSpec, Token const *tok);
int printASTTypeSpec(ASTTypeSpec const *typeSpec);

void initASTDeclarator(ASTDeclarator *declarator);
void freeASTDeclarator(ASTDeclarator *declarator);
int parseASTDeclarator(ASTDeclarator *declarator, Token const *tok);
int printASTDeclarator(ASTDeclarator const *declarator);

void initASTDeclaration(ASTDeclaration *declaration);
void freeASTDeclaration(ASTDeclaration *declaration);
int parseASTDeclaration(ASTDeclaration *declaration, Token const *tok);
int printASTDeclaration(ASTDeclaration const *declaration);

