#pragma once

struct ASTExp;
struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTWhile {
	struct ASTExp *expression;
	struct ASTStm *statement;
} ASTWhile;

void initASTWhile(ASTWhile *node);
void freeASTWhile(ASTWhile *node);
int parseASTWhile(ASTWhile *node, struct Token const *tok, struct ASTScope *scope);
int printASTWhile(ASTWhile const *node);
