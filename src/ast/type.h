#pragma once

#include "declaration.h"
#include "structDecl.h"

typedef enum {
	AST_TT_UNKNOWN,
	AST_TT_ARITH,
	AST_TT_TYPEDEF,
	AST_TT_STRUCT,
	AST_TT_TYPEDEFREF,
} ASTTypeType;

typedef struct ASTType {
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	char *name;
	ASTTypeType type;
	union {
		ASTArithType arith;
		struct ASTType *tdef;
		char *tdefRef;
	} c;
	Token const *tok;
} ASTType;

void initASTType(ASTType *type);
void initASTTypePart(
		ASTType *type,
		ASTTypeSpec const *spec,
		ASTDeclarator const *declarator);
void freeASTType(ASTType *type);
int printASTType(ASTType *type);
int cmpASTType(ASTType const *lhs, ASTType const *rhs);

/*
 * Whether the two types can exist in the same scope
 * If they can, return the preferred type
 */
ASTType *astTypeComp(ASTType *lhs, ASTType *rhs);

