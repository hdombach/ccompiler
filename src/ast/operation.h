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
	AST_OT_COND,
} ASTOperationType;

typedef struct ASTFuncOperation {
	struct ASTExp *func;
	DList params;
} ASTFuncOperation;

/* or ternerary */
typedef struct ASTCondOperation {
	struct ASTExp *condition;
	struct ASTExp *trueExp;
	struct ASTExp *falseExp;
} ASTCondOperation;

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
		struct ASTCondOperation cond;
	} c;
} ASTOperation;

void initASTFuncOperation(ASTFuncOperation *node);
void freeASTFuncOperation(ASTFuncOperation *node);
/* If the parse fails, func is not freed */
int parseASTFuncOperation(ASTFuncOperation *node,  Token const *tok, struct ASTExp *func);
int printASTFuncOperation(ASTFuncOperation const *node);

void initASTCondOperation(ASTCondOperation *node);
void freeASTCondOperation(ASTCondOperation *node);
int parseASTCondOperation(ASTCondOperation *node, Token const *tok);
int printASTCondOperation(ASTCondOperation const *node);

void initASTOperation(ASTOperation *node);
void freeASTOperation(ASTOperation *node);
int parseASTOperation15(ASTOperation *node, Token const *tok);
int parseASTOperation14(ASTOperation *node, Token const *tok);
int parseASTOperation13(ASTOperation *node, Token const *tok);
int parseASTOperation12(ASTOperation *node, Token const *tok);
int parseASTOperation11(ASTOperation *node, Token const *tok);
int parseASTOperation10(ASTOperation *node, Token const *tok);
int parseASTOperation9(ASTOperation *node, Token const *tok);
int parseASTOperation8(ASTOperation *node, Token const *tok);
int parseASTOperation7(ASTOperation *node, Token const *tok);
int parseASTOperation6(ASTOperation *node, Token const *tok);
int parseASTOperation5(ASTOperation *node, Token const *tok);
int parseASTOperation4(ASTOperation *node, Token const *tok);
int parseASTOperation3(ASTOperation *node, Token const *tok);
int printASTOperation(ASTOperation const *node);
