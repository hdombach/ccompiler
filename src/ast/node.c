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

void initASTNode(ASTNode *node, struct Token const *tok) {
	node->type = AST_UNKNOWN;
	node->parent = NULL;
	node->curScope = NULL;
	node->tok = tok;
}

void freeASTNode(ASTNode *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: break;
		case AST_FLOAT_CONSTANT: break;
		case AST_CHAR_CONSTANT: break;
		case AST_IDENTIFIER_DECL:
		case AST_IDENTIFIER_TS:
		case AST_IDENTIFIER: free(((ASTIdentifier *) node)->name); break;
		case AST_FUNC_OPERATION: freeASTFuncOperation((ASTFuncOperation *) node); break;
		case AST_COND_OPERATION: freeASTCondOperation((ASTCondOperation *) node); break;
		case AST_SUBS_OPERATION:
		case AST_CAST_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: freeASTOperation((ASTOperation *)node); break;
		case AST_PARAM: freeASTParam((ASTParam *) node); break;
		case AST_DECLARATION: freeASTDeclaration((ASTDeclaration *) node); break;
		case AST_ARRAY_DECL: freeASTArrayDecl((ASTArrayDecl *) node); break;
		case AST_FUNC_DECL: freeASTFuncDecl((ASTFuncDecl *) node); break;
		case AST_POINTER_DECL:
		case AST_DECLARATOR: freeASTDeclarator((ASTDeclarator *) node); break;
		case AST_TYPE_SPEC: freeASTTypeSpec((ASTTypeSpec *) node); break;
		case AST_STRUCT_DECL: freeASTStructDecl((ASTStructDecl *) node); break;
		case AST_ENUM_DECL: freeASTEnumDecl((ASTEnumDecl *) node); break;
		case AST_ENUM_CONST: freeASTEnumConst((ASTEnumConst *) node); break;
		case AST_STM: freeASTStm((ASTStm *) node); break;
		case AST_COMP_STM: freeASTCompStm((ASTCompStm *) node); break;
		case AST_IF: freeASTIf((ASTIf *) node); break;
		case AST_SWITCH: freeASTSwitch((ASTSwitch *) node); break;
		case AST_WHILE: freeASTWhile((ASTWhile *) node); break;
		case AST_DO_WHILE: freeASTDoWhile((ASTDoWhile *) node); break;
		case AST_EMPTY_STM:
		case AST_BREAK: 
		case AST_CONTINUE: break;
		case AST_FUNC_DEF: freeASTFuncDef((ASTFuncDef *) node); break;
		case AST_FILE: freeASTFile((ASTFile *) node); break;
		case AST_INITIALIZER_LIST: freeASTInitializerList((ASTInitializerList *) node); break;
		case AST_LBL_IDENTIFIER: freeASTLblIdentifier((ASTLblIdentifier *) node); break;
		case AST_LBL_CASE: freeASTLblCase((ASTLblCase *) node); break;
		case AST_LBL_DEFAULT: break;
		case AST_FOR: freeASTFor((ASTFor *) node); break;
		case AST_GOTO: freeASTGoto((ASTGoto *) node); break;
		default: break;
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

const char *_astNodeTypes[] = {
	"unknown",
	"int constant",
	"float constant",
	"char constant",
	"identifier",
	"function call operation",
	"subscript operation",
	"conditional operation",
	"type case operation",
	"sizeof type operation",
	"sizeof expression operation",
	"binary operation",
	"prefix operation",
	"postfix operation",
	"param",
	"declaration",
	"array declarator",
	"function declarator",
	"identifier declarator",
	"pointer declarator",
	"declarator",
	"type specifier",
	"typedef name type specifier",
	"struct declaration",
	"enum declaration",
	"enumerator declaration const",
	"statement",
	"compound statement",
	"if statement",
	"switch statement",
	"while statement",
	"do while statement",
	"empty statement",
	"break statement",
	"continue statement",
	"function definition",
	"file",
	"initializer list",
	"identifier label",
	"case label",
	"default label",
	"for statement",
	"goto statement",
};

const char *astNodeTypeStr(ASTNodeType type) {
	return _astNodeTypes[type];
}



int printASTNode(ASTNode const *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: return printASTIntContant((ASTIntConstant *) node);
		case AST_FLOAT_CONSTANT: return printASTFloatConstant((ASTFloatConstant *) node);
		case AST_CHAR_CONSTANT: return printAStCharConstant((ASTCharConstant *) node);
		case AST_IDENTIFIER_DECL:
		case AST_IDENTIFIER_TS:
		case AST_IDENTIFIER: return printASTIdentifier((ASTIdentifier *) node);
		case AST_FUNC_OPERATION: return printASTFuncOperation((ASTFuncOperation *) node);
		case AST_COND_OPERATION: return printASTCondOperation((ASTCondOperation *) node);
		case AST_SUBS_OPERATION:
		case AST_CAST_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: return printASTOperation((ASTOperation *) node);
		case AST_PARAM: return printASTParam((ASTParam *) node);
		case AST_DECLARATION: return printASTDeclaration((ASTDeclaration *) node);
		case AST_ARRAY_DECL: return printASTArrayDecl((ASTArrayDecl *) node);
		case AST_FUNC_DECL: return printASTFuncDecl((ASTFuncDecl *) node);
		case AST_POINTER_DECL:
		case AST_DECLARATOR: return printASTDeclarator((ASTDeclarator *) node);
		case AST_TYPE_SPEC: return printASTTypeSpec((ASTTypeSpec *) node);
		case AST_STRUCT_DECL: return printASTStructDecl((ASTStructDecl *) node);
		case AST_ENUM_DECL: return printASTEnumDecl((ASTEnumDecl *) node);
		case AST_ENUM_CONST: return printASTEnumConst((ASTEnumConst *) node);
		case AST_STM: return printASTStm((ASTStm *) node);
		case AST_COMP_STM: return printASTCompStm((ASTCompStm *) node);
		case AST_IF: return printASTIf((ASTIf *) node);
		case AST_SWITCH: return printASTSwitch((ASTSwitch *) node);
		case AST_WHILE: return printASTWhile((ASTWhile *) node);
		case AST_DO_WHILE: return printASTDoWhile((ASTDoWhile *) node);
		case AST_EMPTY_STM: return printASTEmptyStm((ASTEmptyStm *) node);
		case AST_BREAK: return printASTBreak((ASTBreak *) node);
		case AST_CONTINUE: return printASTContinue((ASTContinue *) node);
		case AST_FUNC_DEF: return printASTFuncDef((ASTFuncDef *) node);
		case AST_FILE: return printASTFile((ASTFile *) node);
		case AST_INITIALIZER_LIST: return printASTInitializerList((ASTInitializerList *) node);
		case AST_LBL_IDENTIFIER: return printASTLblIdentifier((ASTLblIdentifier *) node);
		case AST_LBL_CASE: return printASTLblCase((ASTLblCase *) node);
		case AST_LBL_DEFAULT: return printASTLblDefault(node);
		case AST_FOR: return printASTFor((ASTFor *) node);
		case AST_GOTO: return printASTGoto((ASTGoto *) node);
		case AST_UNKNOWN: return printf("\"(unknown node)\"");
	}
}

int astNodeChildCount(const ASTNode *node) {
	switch (node->type) {
		case AST_INT_CONSTANT: return astIntConstantChildCount((ASTIntConstant *) node);
		case AST_FLOAT_CONSTANT: return astFloatConstantChildCount((ASTFloatConstant *) node);
		case AST_CHAR_CONSTANT: return astCharConstantChildCount((ASTCharConstant *) node);
		case AST_IDENTIFIER: return astIdentifierChildCount((ASTIdentifier *) node);
		case AST_FUNC_OPERATION: return astFuncOperationChildCount((ASTFuncOperation *) node);
		case AST_SUBS_OPERATION: return astOperationChildCount((ASTOperation *) node);
		case AST_COND_OPERATION: return astCondOperationChildCount((ASTCondOperation *) node);
		case AST_CAST_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: return astOperationChildCount((ASTOperation *) node);
		case AST_PARAM: return astParamChildCount((ASTParam *) node);
		case AST_DECLARATION: return astDeclarationChildCount((ASTDeclaration *) node);
		case AST_ARRAY_DECL: return astArrayDeclChildCount((ASTArrayDecl *) node);
		case AST_FUNC_DECL: return astFuncDeclChildCount((ASTFuncDecl *) node);
		case AST_IDENTIFIER_DECL: return astIdentifierChildCount((ASTIdentifier *) node);
		case AST_POINTER_DECL:
		case AST_DECLARATOR: return astDeclaratorChildCount((ASTDeclarator *) node);
		case AST_TYPE_SPEC: return astTypeSpecChildCount((ASTTypeSpec *) node);
		case AST_IDENTIFIER_TS: return astIdentifierChildCount((ASTIdentifier *) node);
		case AST_STRUCT_DECL: return astStructDeclChildCount((ASTStructDecl *) node);
		case AST_ENUM_DECL: return astEnumDeclChildCount((ASTEnumDecl *) node);
		case AST_ENUM_CONST: return astEnumConstChildCount((ASTEnumConst *) node);
		case AST_STM: return astStmChildCount((ASTStm *) node);
		case AST_COMP_STM: return astCompStmChildCount((ASTCompStm *) node);
		case AST_IF: return astIfChildCount((ASTIf *) node);
		case AST_SWITCH: return astSwitchChildCount((ASTSwitch *) node);
		case AST_WHILE: return astWhileChildCount((ASTWhile *) node);
		case AST_DO_WHILE: return astDoWhileChildCount((ASTDoWhile *) node);
		case AST_EMPTY_STM: return astEmptyStmChildCount((ASTEmptyStm *) node);
		case AST_BREAK: return astBreakChildCount((ASTBreak *) node);
		case AST_CONTINUE: return astContinueChildCount((ASTContinue *) node);
		case AST_FUNC_DEF: return astFuncDefChildCount((ASTFuncDef *) node);
		case AST_FILE: return astFileChildCount((ASTFile *) node);
		case AST_INITIALIZER_LIST: return astInitializerListChildCount((ASTInitializerList *) node);
		case AST_LBL_IDENTIFIER: return astLblIdentifierChildCount((ASTLblIdentifier *) node);
		case AST_LBL_CASE: return astLblCaseChildCount((ASTLblCase *) node);
		case AST_LBL_DEFAULT: return astLblDefaultChildCount(node);
		case AST_FOR: return astForChildCount((ASTFor *) node);
		case AST_GOTO: return astGotoChildCount((ASTGoto *) node);
		case AST_UNKNOWN: return 0;
	}
}

ASTNode *astNodeGetChild(ASTNode *node, int index) {
	switch (node->type) {
		case AST_INT_CONSTANT: return astIntConstantGetChild((ASTIntConstant *) node, index);
		case AST_FLOAT_CONSTANT: return astFloatConstantGetChild((ASTFloatConstant *) node, index);
		case AST_CHAR_CONSTANT: return astCharConstantGetChild((ASTCharConstant *) node, index);
		case AST_IDENTIFIER: return astIdentifierGetChild((ASTIdentifier *) node, index);
		case AST_FUNC_OPERATION: return astFuncOperationGetChild((ASTFuncOperation *) node, index);
		case AST_SUBS_OPERATION: return astOperationGetChild((ASTOperation *) node, index);
		case AST_COND_OPERATION: return astCondOperationGetChild((ASTCondOperation *) node, index);
		case AST_CAST_OPERATION:
		case AST_SIZEOF_TYPE_OPERATION:
		case AST_SIZEOF_EXP_OPERATION:
		case AST_BINARY_OPERATION:
		case AST_PREFIX_OPERATION:
		case AST_POSTFIX_OPERATION: return astOperationGetChild((ASTOperation *) node, index);
		case AST_PARAM: return astParamGetChild((ASTParam *) node, index);
		case AST_DECLARATION: return astDeclarationGetChild((ASTDeclaration *) node, index);
		case AST_ARRAY_DECL: return astArrayDeclGetChild((ASTArrayDecl *) node, index);
		case AST_FUNC_DECL: return astFuncDeclGetChild((ASTFuncDecl *) node, index);
		case AST_IDENTIFIER_DECL: return astIdentifierGetChild((ASTIdentifier *) node, index);
		case AST_POINTER_DECL:
		case AST_DECLARATOR: return astDeclaratorGetChild((ASTDeclarator *) node, index);
		case AST_TYPE_SPEC: return astTypeSpecGetChild((ASTTypeSpec *) node, index);
		case AST_IDENTIFIER_TS: return astIdentifierGetChild((ASTIdentifier *) node, index);
		case AST_STRUCT_DECL: return astStructDeclGetChild((ASTStructDecl *) node, index);
		case AST_ENUM_DECL: return astEnumDeclGetChild((ASTEnumDecl *) node, index);
		case AST_ENUM_CONST: return astEnumConstGetChild((ASTEnumConst *) node, index);
		case AST_STM: return astStmGetChild((ASTStm *) node, index);
		case AST_COMP_STM: return astCompStmGetChild((ASTCompStm *) node, index);
		case AST_IF: return astIfGetChild((ASTIf *) node, index);
		case AST_SWITCH: return astSwitchGetChild((ASTSwitch *) node, index);
		case AST_WHILE: return astWhileGetChild((ASTWhile *) node, index);
		case AST_DO_WHILE: return astDoWhileGetChild((ASTDoWhile *) node, index);
		case AST_EMPTY_STM: return astEmptyStmGetChild((ASTEmptyStm *) node, index);
		case AST_BREAK: return astBreakGetChild((ASTBreak *) node, index);
		case AST_CONTINUE: return astContinueGetChild((ASTContinue *) node, index);
		case AST_FUNC_DEF: return astFuncDefGetChild((ASTFuncDef *) node, index);
		case AST_FILE: return astFileGetChild((ASTFile *) node, index);
		case AST_INITIALIZER_LIST: return astInitializerListGetChild((ASTInitializerList *) node, index);
		case AST_LBL_IDENTIFIER: return astLblIdentifierGetChild((ASTLblIdentifier *) node, index);
		case AST_LBL_CASE: return astLblCaseGetChild((ASTLblCase *) node, index);
		case AST_LBL_DEFAULT: return astLblDefaultGetChild(node, index);
		case AST_FOR: return astForGetChild((ASTFor *) node, index);
		case AST_GOTO: return astGotoGetChild((ASTGoto *) node, index);
		case AST_UNKNOWN: return 0;
	}
}

ASTScope *astNodeScope(ASTNode *node, ASTScope *defaultScope) {
	if (node->type == AST_COMP_STM) {
		ASTCompStm *stm = (ASTCompStm *) node;
		return stm->scope;
	} else if (node->type == AST_FILE) {
		ASTFile *file = (ASTFile *) node;
		return file->scope;
	} else if (node->type == AST_FUNC_DECL) {
		ASTFuncDecl *funcDecl = (ASTFuncDecl *) node;
		return funcDecl->scope;
	} else if (node->type == AST_FUNC_DEF) {
		ASTFuncDef *funcDef = (ASTFuncDef *) node;
		if (!ASSERT(funcDef->node.type == AST_FUNC_DEF)) return NULL;

		ASTDeclarator *declarator = (ASTDeclarator *) funcDef->funcDecl;
		if (!ASSERT(declarator->node.type == AST_DECLARATOR)) return NULL;

		ASTFuncDecl *funcDecl = (ASTFuncDecl *) declarator->encl;
		if (!ASSERT(funcDecl->node.type == AST_FUNC_DECL)) return NULL;

		return funcDecl->scope;
	} else if (node->type == AST_STRUCT_DECL) {
		ASTStructDecl *structDecl = (ASTStructDecl *) node;
		return structDecl->scope;
	} else {
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
