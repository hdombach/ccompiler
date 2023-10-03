#pragma once

struct ASTNode;
struct Token;
struct ASTScope;

typedef enum ASTLabelType {
	AST_LT_UNKNOWN,
	AST_LT_IDENTIFIER,
	AST_LT_CASE,
	AST_LT_DEFAULT,
} ASTLabelType;

typedef struct ASTLabel {
	ASTLabelType type;
	union {
		char *identifier;
		struct ASTNode *expression;
	} c;
} ASTLabel;

void initASTLabel(ASTLabel *label);
void freeASTLabel(ASTLabel *label);
int parseASTLabel(
		ASTLabel *label,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTLabel(ASTLabel const *label);
