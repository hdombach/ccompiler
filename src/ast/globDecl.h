#pragma once

#include "../token.h"

typedef enum {
	ASTG_UNKNOWN,
	ASTG_FUNC,
} ASTGlobDeclType;

/*
 * Not actuall statements.
 * Things that can appear in the root scope
 */
typedef struct {
	ASTGlobDeclType type;
	union {
	} c;
} ASTGlobDecl;

void initASTGlobStm(ASTGlobDecl *node);
void freeASTGlobStm(ASTGlobDecl *node);

int parseASTGlobStm(ASTGlobDecl *node, Token const *tok);

int printASTGlobStm(ASTGlobDecl *node);

void testThing(int value);
