#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "arrayDecl.h"
#include "enumDecl.h"
#include "expression.h"
#include "identifier.h"
#include "initializer.h"
#include "structDecl.h"
#include "funcDecl.h"
#include "node.h"

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
	AST_AT_FLOAT = 0b1000000,
	AST_AT_DOUBLE = 0b10000000,
} ASTArithType;

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
	ASTNode node;
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	ASTTypeSpecType typeSpecType;
	ASTArithType arith;
	ASTNode *content;
	Token const *tok;
} ASTTypeSpec;

/*
 * Note:
 * For declarators, the nesting of the decleration is reverse of the 
 * nesting of the type.
 * *test[5] (pointer { array }) Is actually array of pointers
 */

typedef struct ASTDeclarator {
	ASTNode node;
	/*
	 * Can be AST_POINTER_DECL, AST_ARRAY_DECL, AST_FUNC_DECL, AST_IDENTIFIER_DECL
	 */
	ASTNode *encl;
	/* Used when representing contents of pointer*/
	ASTTypeQualifier qualifiers;
	ASTNode *initializer;
	ASTNode *bitField;
} ASTDeclarator;

typedef struct ASTDeclaration {
	ASTNode node;
	ASTTypeSpec *typeSpec;
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

void initASTTypeSpec(ASTTypeSpec *typeSpec, Token const *tok);
void freeASTTypeSpec(ASTTypeSpec *typeSpec);
int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		Token const *tok,
		ASTScope const *scope);
int printASTTypeSpec(ASTTypeSpec const * typeSpec);

void initASTDeclarator(ASTDeclarator *declarator, Token const *tok);
void freeASTDeclarator(ASTDeclarator *declarator);
int parseASTDeclarator(
		ASTDeclarator *declarator,
		Token const *tok,
		ASTScope const *scope);
int printASTDeclarator(ASTDeclarator const *declarator);

void initASTDeclaration(ASTDeclaration *declaration, Token const *tok);
void freeASTDeclaration(ASTDeclaration *declaration);
int parseASTDeclaration(
		ASTDeclaration *declaration,
		Token const *tok,
		ASTScope const *scope);
int printASTDeclaration(ASTDeclaration const *declaration);

DList astDeclarationTypedefNames(ASTDeclaration const *declaration);
/* Caller needs to free result */
char * astDeclaratorTypedefName(ASTDeclarator const *declarator);
