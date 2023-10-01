#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "node.h"

struct ASTExp;
struct ASTDeclaration;
struct ASTParam;
struct ASTScope;

typedef struct ASTFuncOperation {
	ASTNode node;
	/* expression */
	ASTNode *func;
	DList params;
} ASTFuncOperation;

/* or ternerary */
typedef struct ASTCondOperation {
	ASTNode node;
	ASTNode *condition;
	ASTNode *trueExp;
	ASTNode *falseExp;
} ASTCondOperation;

/*
 * Is used for Subscript, cast, sizeof, binary, prefix, postfix
 */
typedef struct ASTOperation {
	ASTNode node;
	TokenType tokType;
	ASTNode *lhs;
	ASTNode *rhs;
	char buf[50];
} ASTOperation;


void initASTFuncOperation(ASTFuncOperation *node);
void freeASTFuncOperation(ASTFuncOperation *node);
/* If the parse fails, func is not freed */
int parseASTFuncOperation(
		ASTFuncOperation *node,
		Token const *tok,
		ASTNode *func,
		struct ASTScope const *scope);
int printASTFuncOperation(ASTFuncOperation const *node);

void initASTCondOperation(ASTCondOperation *node);
void freeASTCondOperation(ASTCondOperation *node);
int parseASTCondOperation(
		ASTCondOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int printASTCondOperation(ASTCondOperation const *node);

void initASTOperation(ASTOperation *node);
void freeASTOperation(ASTOperation *node);
int parseASTOperation15(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation14(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation13(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation12(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation11(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation10(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation9(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation8(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation7(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation6(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation5(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation4(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation3(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation2(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope const *scope);
int parseASTOperation1(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope const *scope,
		struct ASTNode *lhs);
int printASTOperation(ASTOperation const *node);
