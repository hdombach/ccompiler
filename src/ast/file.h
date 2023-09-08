#pragma once

#include "declaration.h"

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
} ASTFile;

void initASTFileItem(ASTFileItem *item);
void freeASTFileItem(ASTFileItem *item);
int parseASTFileItem(ASTFileItem *item, Token const *tok);
int printASTFileItem(ASTFileItem const *item);

void initASTFile(ASTFile *file);
void freeASTFile(ASTFile *file);
int parseASTFile(ASTFile *file, Token const *tok);
int printASTFile(ASTFile const *file);
