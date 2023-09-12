#pragma once

#include "declaration.h"

typedef enum {
	AST_TT_UNKNOWN,
	AST_TT_ARITH,
} ASTTypeType;

typedef struct ASTType {
	ASTTypeQualifier qualifiers;
	ASTStorageClassSpec storage;
	char *name;
	ASTTypeType type;
	union {
		ASTArithType arith;
	} c;
} ASTType;

void initASTType(ASTType *type);
void initASTTypePart(
		ASTType *type,
		ASTTypeSpec const *spec,
		ASTDeclarator const *declarator);
void freeASTType(ASTType *type);
int printASTType(ASTType *type);

ASTType **getASTTypes(ASTDeclaration const *declaration);

