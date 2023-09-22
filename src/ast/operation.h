#pragma once

#include "../util/dlist.h"
#include "../token.h"

struct ASTExp;
struct ASTDeclaration;

typedef enum ASTOperationType {
	AST_OT_UNKNOWN,
	AST_OT_BINARY,
	AST_OT_PREFIX,
	AST_OT_POSTFIX,
	AST_OT_FUNC,
	AST_OT_TYPECAST,
} ASTOperationType;

typedef struct ASTFuncOperation {
	struct ASTExp *func;
	DList params;
} ASTFuncOperation;

typedef struct ASTOperation {
	ASTOperationType type;
	TokenType tokType;
	union {
		struct {
			struct ASTExp *lhs;
			struct ASTExp *rhs;
		} bin;
		struct ASTExp *unary;
		ASTFuncOperation func;
		struct ASTDeclaration *typeCast;
	} c;
} ASTOperation;

void initASTFuncOperation(ASTFuncOperation *node);
void freeASTFuncOperation(ASTFuncOperation *node);
/* If the parse fails, func is not freed */
int parseASTFuncOperation(ASTFuncOperation *node,  Token const *tok, struct ASTExp *func);
int printASTFuncOperation(ASTFuncOperation const *node);

void initASTOperation(ASTOperation *node);
void freeASTOperation(ASTOperation *node);
int parseASTOperation15(ASTOperation *node, Token const *tok);
int parseASTOperation14(ASTOperation *node, Token const *tok);
int printASTOperation(ASTOperation const *node);
