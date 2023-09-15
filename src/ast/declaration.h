#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "enumDecl.h"
#include "expression.h"
#include "initializer.h"
#include "structDecl.h"

// https://en.cppreference.com/w/c/language/declarations

typedef struct ASTScope ASTScope;

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

//TODO: add float
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
	/* The declaration references void */
	AST_TST_VOID,
	/* The declaration referneces a number */
	AST_TST_ARITH,
	/* The declaration references a typedef */
	AST_TST_TYPEDEF,
	/* The declaration introduces or references a struct */
	AST_TST_STRUCT,
	/* The declaration introduces or references an union */
	AST_TST_UNION,
	/* The declaration introduces or references an enum */
	AST_TST_ENUM,
} ASTTypeSpecType;

/*
 * A type specifier
 * (The first chunk of a declaration)
 */
typedef struct {
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	ASTTypeSpecType typeSpecType;
	union {
		ASTArithType arith;
		char *typedefName;
		ASTStructDecl structDecl;
		ASTEnumDecl enumDecl;
	} c;
	Token const *tok;
} ASTTypeSpec;

//TODO: add other declerator types
//TODO: add initialization thing
/*
 * Note:
 * For declarators, the nesting of the decleration is reverse of the 
 * nesting of the type.
 * *test[5] (pointer { array }) Is actually array of pointers
 */
typedef enum {
	AST_DT_UNKNOWN,
	AST_DT_IDENTIFIER,
	AST_DT_POINTER,
} ASTDeclaratorType;

typedef struct ASTDeclarator {
	ASTDeclaratorType type;
	union {
		char *identifier;
		struct ASTDeclarator *pointer;
	} c;
	ASTInitializer *initializer;
	ASTExp *bitField;
} ASTDeclarator;

typedef struct ASTDeclaration {
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
int astArithTypeNormalize(ASTArithType const *type);

void initASTTypeSpec(ASTTypeSpec *typeSpec);
void freeASTTypeSpec(ASTTypeSpec *typeSpec);
int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		Token const *tok,
		ASTScope const *scope);

void initASTDeclarator(ASTDeclarator *declarator);
void freeASTDeclarator(ASTDeclarator *declarator);
int parseASTDeclarator(ASTDeclarator *declarator, Token const *tok);
int printASTDeclarator(ASTDeclarator const *declarator);

void initASTDeclaration(ASTDeclaration *declaration);
void freeASTDeclaration(ASTDeclaration *declaration);
int parseASTDeclaration(
		ASTDeclaration *declaration,
		Token const *tok,
		ASTScope const *scope);
int printASTDeclaration(ASTDeclaration const *declaration);

DList astDeclarationTypedefNames(ASTDeclaration const *declaration);
/* Caller needs to free result */
char * astDeclaratorTypedefName(ASTDeclarator const *declarator);
