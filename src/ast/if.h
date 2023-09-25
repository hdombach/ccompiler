#pragma once

struct ASTExp;
struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTIf {
	struct ASTExp *expression;
	struct ASTStm *trueStatement;
	struct ASTStm *falseStatement;
} ASTIf;

void initASTIf(ASTIf *node);
void freeASTIf(ASTIf *node);
int parseASTIf(ASTIf *node, struct Token const *tok, struct ASTScope *scope);
int printASTIf(ASTIf const *node);
