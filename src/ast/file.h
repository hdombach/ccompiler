#pragma once

#include "declaration.h"
#include "scope.h"

typedef enum {
	AST_FIT_UNKNOWN,
	AST_FIT_DECL,
} ASTFileItemType;

typedef struct {
	ASTFileItemType type;
	union {
		ASTDeclaration declaration;
	} c;
} ASTFileItem;

typedef struct {
	DList items;
	ASTScope scope;
} ASTFile;

void initASTFileItem(ASTFileItem *item);
void freeASTFileItem(ASTFileItem *item);
int parseASTFileItem(ASTFileItem *item, Token const *tok, ASTScope const *scope);
int printASTFileItem(ASTFileItem const *item);

DList astFileItemTypes(ASTFileItem const *item);

void initASTFile(ASTFile *file);
void freeASTFile(ASTFile *file);
int parseASTFile(ASTFile *file, Token const *tok);
int printASTFile(ASTFile const *file);
