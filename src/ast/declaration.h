#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "specialDecl.h"
#include "expression.h"
#include "identifier.h"
#include "initializer.h"
#include "node.h"

// https://en.cppreference.com/w/c/language/declarations

typedef struct ASTScope ASTScope;

/* =========================================================================
 * ASTTypeQualifier
 * ========================================================================= */

typedef enum  {
	AST_TQ_NONE = 0b0,
	AST_TQ_CONST = 0b1,
	AST_TQ_VOLATILE = 0b10,
} ASTTypeQualifier;

void initASTTypeQualifier(ASTTypeQualifier *qualifiers);
int parseASTTypeQualifier(ASTTypeQualifier *qualifiers, Token const *tok);
int printASTTypeQualifier(ASTTypeQualifier const *qualifiers);

/* =========================================================================
 * ASTStorageClassSpec 
 * ========================================================================= */

typedef enum {
	AST_SC_NONE = 0b0,
	AST_SC_TYPEDEF = 0b1,
	AST_SC_AUTO = 0b10,
	AST_SC_REGISTER = 0b100,
	AST_SC_STATIC = 0b1000,
	AST_SC_EXTERN = 0b10000,
} ASTStorageClassSpec;

void initASTStorageClassSpec(ASTStorageClassSpec *specs);
int parseASTStorageClassSpec(ASTStorageClassSpec *specs, Token const *tok);
int printASTStorageClassSpec(ASTStorageClassSpec const *specs);

/* =========================================================================
 * ASTArithType
 * ========================================================================= */

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

void initASTArithType(ASTArithType *type);
int parseASTArithType(ASTArithType *type, Token const *tok);
int printASTArithType(ASTArithType const *type);
ASTArithType astArithTypeNormalize(ASTArithType type);

/* =========================================================================
 * ASTTypeSpec
 * ========================================================================= */

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
typedef struct ASTTypeSpec {
	ASTNode node;
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	ASTTypeSpecType typeSpecType;
	ASTArithType arith;
	ASTNode *content;
	Token const *tok;
} ASTTypeSpec;

void initASTTypeSpec(ASTTypeSpec *typeSpec, Token const *tok);
void freeASTTypeSpec(ASTTypeSpec *typeSpec);
int parseASTTypeSpec(
		ASTTypeSpec *typeSpec,
		Token const *tok,
		ASTScope *scope);
int printASTTypeSpec(ASTTypeSpec const * typeSpec);
int astTypeSpecChildCount(ASTTypeSpec const *typeSpec);
ASTNode *astTypeSpecGetChild(ASTTypeSpec *typeSpec, int index);

/* =========================================================================
 * ASTDeclarator
 * ========================================================================= */

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

void initASTDeclarator(ASTDeclarator *declarator, Token const *tok);
void freeASTDeclarator(ASTDeclarator *declarator);
int parseASTDeclarator(
		ASTDeclarator *declarator,
		Token const *tok,
		ASTScope *scope);
int printASTDeclarator(ASTDeclarator const *declarator);
int astDeclaratorChildCount(ASTDeclarator const *node);
ASTNode *astDeclaratorGetChild(ASTDeclarator *node, int index);

/* =========================================================================
 * ASTDeclaration
 * ========================================================================= */

typedef struct ASTDeclaration {
	ASTNode node;
	ASTTypeSpec *typeSpec;
	DList declarators;
} ASTDeclaration;

void initASTDeclaration(ASTDeclaration *declaration, Token const *tok);
void freeASTDeclaration(ASTDeclaration *declaration);
int parseASTDeclaration(
		ASTDeclaration *declaration,
		Token const *tok,
		ASTScope *scope);
int printASTDeclaration(ASTDeclaration const *declaration);
int astDeclarationChildCount(ASTDeclaration const *node);
ASTNode *astDeclarationGetChild(ASTDeclaration *node, int index);


DList astDeclarationTypedefNames(ASTDeclaration const *declaration);
/* Caller needs to free result */
const char * astDeclaratorName(ASTNode const *declarator);
