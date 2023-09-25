#pragma once

struct ASTExp;
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
		struct ASTExp *expression;
	} c;
} ASTLabel;

void initASTLabel(ASTLabel *label);
void freeASTLabel(ASTLabel *label);
int parseASTLabel(
		ASTLabel *label,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTLabel(ASTLabel const *label);
