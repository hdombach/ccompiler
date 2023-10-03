#pragma once

struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTDoWhile {
	struct ASTNode *expression;
	struct ASTStm *statement;
} ASTDoWhile;

void initASTDoWhile(ASTDoWhile *node);
void freeASTDoWhile(ASTDoWhile *node);
int parseASTDoWhile(
		ASTDoWhile *node,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTDoWhile(ASTDoWhile const *node);
