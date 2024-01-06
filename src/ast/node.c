#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "specialDecl.h"
#include "compStatement.h"
#include "declaration.h"
#include "expression.h"
#include "file.h"
#include "funcDef.h"
#include "identifier.h"
#include "selection.h"
#include "iteration.h"
#include "initializer.h"
#include "intConstant.h"
#include "floatConst.h"
#include "charConstant.h"
#include "label.h"
#include "node.h"
#include "operation.h"
#include "param.h"
#include "statement.h"
#include "../util/log.h"
#include "stringConstant.h"

void initASTNode(
		ASTNode *node,
		struct Token const *tok,
		ASTNodeVTable const *vtable)
{
	node->type = AST_UNKNOWN;
	node->parent = NULL;
	node->curScope = NULL;
	node->tok = tok;
	node->vtable = vtable;
}

void freeASTNode(ASTNode *node) {
	if (node->vtable && node->vtable->table.freeFunc) {
		node->vtable->table.freeFunc(node);
	}

	node->type = AST_UNKNOWN;
}

void astNodeSetParent(ASTNode *node, ASTNode *parent) {
	node->parent = node->parent;
	node->curScope = parent->curScope;
}

void mvASTNode(ASTNode *dest, ASTNode *src) {
	memcpy(dest, src, AST_NODE_S);
	src->type = AST_UNKNOWN;
}

ASTNode *dupASTNode(ASTNode *node) {
	ASTNode *result = malloc(AST_NODE_S);
	mvASTNode(result, node);
	return result;
}

#define X(NAME, STR) STR,
const char *_astNodeTypes[] = {
	X_AST_NODE_TYPE
};
#undef X

const char *astNodeTypeStr(ASTNodeType type) {
	return _astNodeTypes[type];
}



int printASTNode(ASTNode const *node) {
	if (ASSERT(node->vtable && node->vtable->table.printFunc)) {
		return node->vtable->table.printFunc(node);
	} else {
		return 0;
	}
}

int astNodeChildCount(const ASTNode *node) {
	if (ASSERT(node->vtable && node->vtable->childCount)) {
		return node->vtable->childCount(node);
	} else {
		return 0;
	}
}

ASTNode *astNodeGetChild(ASTNode *node, int index) {
	if (ASSERT(node->vtable && node->vtable->getChild)) {
		return node->vtable->getChild(node, index);
	} else {
		return 0;
	}
}

ASTScope *astNodeScope(ASTNode *node, ASTScope *defaultScope) {
	switch (node->type) {
		case AST_COMP_STM:
			{
				ASTCompStm *stm = (ASTCompStm *) node;
				return stm->scope;
			}
		case AST_FILE:
			{
				ASTFile *file = (ASTFile *) node;
				return file->scope;
			}
		case AST_FUNC_DECL:
			{
				ASTFuncDecl *funcDecl = (ASTFuncDecl *) node;
				return funcDecl->scope;
			}
		case AST_FUNC_DEF:
			{
				ASTFuncDef *funcDef = (ASTFuncDef *) node;
				if (!ASSERT(funcDef->node.type == AST_FUNC_DEF)) return NULL;

				ASTDeclarator *declarator = (ASTDeclarator *) funcDef->funcDecl;
				if (!ASSERT(declarator->node.type == AST_DECLARATOR)) return NULL;

				ASTFuncDecl *funcDecl = (ASTFuncDecl *) declarator->encl;
				if (!ASSERT(funcDecl->node.type == AST_FUNC_DECL)) return NULL;

				return funcDecl->scope;
			}
		case AST_STRUCT_DECL:
			{
				ASTStructDecl *structDecl = (ASTStructDecl *) node;
				return structDecl->scope;
			}
		default:
			return defaultScope;
	}
}


ASTTravRes _astNodeTrav(
		ASTNode *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravCtx curCtx;
	curCtx.parent = ctx;
	curCtx.node = node;
	curCtx.parent = ctx;
	if (ctx) curCtx.customCtx = ctx->customCtx;

	curCtx.scope = astNodeScope(node, (ctx ? ctx->scope : NULL));

	ASTTravRes result = ASTT_SUCCESS;;
	if (beforeFunc) {
		result = beforeFunc(node, &curCtx);
		if (result == ASTT_FAILED) {
			return result;
		}
	}

	int child_count = astNodeChildCount(node);
	for (int i = 0; i < child_count; i++) {
		ASTNode *child = astNodeGetChild(node, i);
		if (child) {
			_astNodeTrav(child, beforeFunc, afterFunc, &curCtx);
		}
	}


	if (result == ASTT_FAILED) {
		return result;
	}

	if (afterFunc) {
		result = afterFunc(node, &curCtx);
	}
	return result;
}

ASTTravRes astNodeTrav(
		ASTNode *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		void *customCtx)
{
	ASTTravCtx curCtx = {
		NULL, /* scope */
		NULL, /* node */
		NULL, /* parent */
		customCtx
	};
	return _astNodeTrav(node, beforeFunc, afterFunc, &curCtx);
}
