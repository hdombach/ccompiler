#pragma once

#include "../util/dlist.h"
#include "../token.h"

struct ASTExp;
struct ASTDeclaration;
struct ASTParam;
struct ASTScope;

typedef enum ASTOperationType {
	AST_OT_UNKNOWN,
	AST_OT_BINARY,
	AST_OT_PREFIX,
	AST_OT_POSTFIX,
	AST_OT_FUNC,
	AST_OT_TYPECAST,
	AST_OT_COND,
	AST_OT_SIZEOF,
	AST_OT_SUBSCRIPT,
} ASTOperationType;

typedef struct ASTFuncOperation {
	struct ASTExp *func;
	DList params;
} ASTFuncOperation;

typedef struct ASTSubscriptOperation {
	struct ASTExp *lhs;
	struct ASTExp *index;
} ASTSubscriptOperation;

/* or ternerary */
typedef struct ASTCondOperation {
	struct ASTExp *condition;
	struct ASTExp *trueExp;
	struct ASTExp *falseExp;
} ASTCondOperation;

typedef struct ASTCastOperation {
	struct ASTParam *type;
	struct ASTExp *exp;
} ASTCastOperation;

typedef struct ASTSizeofOperation {
	int isExp;
	union {
		struct ASTExp *exp;
		struct ASTParam *param;
	} c;
} ASTSizeofOperation;

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
		ASTCastOperation typeCast;
		struct ASTCondOperation cond;
		ASTSizeofOperation sizeofOp;
		struct ASTSubscriptOperation subscript;
	} c;
} ASTOperation;


void initASTFuncOperation(ASTFuncOperation *node);
void freeASTFuncOperation(ASTFuncOperation *node);
/* If the parse fails, func is not freed */
int parseASTFuncOperation(
		ASTFuncOperation *node,
		Token const *tok,
		struct ASTExp func,
		struct ASTScope *scope);
int printASTFuncOperation(ASTFuncOperation const *node);

void initASTSubscriptOperation(ASTSubscriptOperation *node);
void freeASTSubscriptOperation(ASTSubscriptOperation *node);
int parseASTSubscriptOperation(
		ASTSubscriptOperation *node,
		Token const *tok,
		struct ASTExp lhs,
		struct ASTScope *scope);
int printASTSubscriptOperation(ASTSubscriptOperation const *node);

void initASTCondOperation(ASTCondOperation *node);
void freeASTCondOperation(ASTCondOperation *node);
int parseASTCondOperation(ASTCondOperation *node, Token const *tok, struct ASTScope *scope);
int printASTCondOperation(ASTCondOperation const *node);

void initASTCastOperation(ASTCastOperation *node);
void freeASTCastOperation(ASTCastOperation *node);
int parseASTCastOperation(
		ASTCastOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int printASTCastOperation(ASTCastOperation const *node);

void initASTSizeofOperation(ASTSizeofOperation *node);
void freeASTSizeofOperation(ASTSizeofOperation *node);
int parseASTSizeofOperation(ASTSizeofOperation *node, Token const *tok, struct ASTScope *scope);
int printASTSizeofOperation(ASTSizeofOperation const *node);

void initASTOperation(ASTOperation *node);
void freeASTOperation(ASTOperation *node);
int parseASTOperation15(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation14(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation13(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation12(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation11(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation10(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation9(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation8(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation7(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation6(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation5(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation4(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation3(ASTOperation *node, Token const *tok, struct ASTScope *scope);
int parseASTOperation2(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation1(ASTOperation *node, const Token *tok, struct ASTScope *scope, struct ASTExp lhs);
int printASTOperation(ASTOperation const *node);
