#include "funcDef.h"
#include "../util/dlist.h"
#include "astUtil.h"
#include "compStatement.h"
#include "declaration.h"
#include "node.h"
#include "param.h"
#include <stdio.h>
#include <stdlib.h>

void initASTFuncDef(ASTFuncDef *def, Token const *tok) {
	initASTNode((ASTNode *) def, tok);
	def->typeSpec = NULL;
	def->funcDecl = NULL;
	def->compoundStm = NULL;
}

void freeASTFuncDef(ASTFuncDef *def) {
	if (def->typeSpec) {
		freeASTTypeSpec(def->typeSpec);
		free(def->typeSpec);
		def->typeSpec = NULL;
	}
	if (def->funcDecl) {
		freeASTNode(def->funcDecl);
		free(def->funcDecl);
		def->funcDecl = NULL;
	}
	if (def->compoundStm) {
		freeASTNode((ASTNode *) def->compoundStm);
		free(def->compoundStm);
	}
}

int parseASTFuncDef(
		ASTFuncDef *def,
		Token const * tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTFuncDef);
	int n = 0, res;
	ASTNode *curDecl, *prevDecl;
	ASTNodeBuf tempBuf;

	initASTFuncDef(def, tok);

	if (astHasErr()) {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTTypeSpec((ASTTypeSpec *) &tempBuf, tok + n, scope))) {
		def->typeSpec = (ASTTypeSpec *) dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
		def->funcDecl = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTFuncDef(def);
		return 0;
	}

	prevDecl = NULL;
	curDecl = def->funcDecl;
	while (curDecl) {
		prevDecl = curDecl;
		switch (curDecl->type) {
			case AST_POINTER_DECL:
			case AST_DECLARATOR:
				curDecl = ((ASTDeclarator *) curDecl)->encl;
				break;
			case AST_ARRAY_DECL:
				curDecl = (ASTNode *) ((ASTArrayDecl *) curDecl)->encl;
				break;
			case AST_FUNC_DECL:
				curDecl = (ASTNode *) ((ASTFuncDecl *) curDecl)->encl;
				if (curDecl && curDecl->type == AST_IDENTIFIER_DECL) {
					curDecl = NULL;
				}
				break;
			default:
				curDecl = NULL;
				break;
		}
	}


	if (!prevDecl || prevDecl->type != AST_FUNC_DECL) {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTCompStm((ASTCompStm *) &tempBuf, tok + n, scope))) {
		def->compoundStm = (ASTCompStm *) dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		//Don't throw error
		freeASTFuncDef(def);
		return 0;
	}

	def->node.type = AST_FUNC_DEF;
	return n;
}

int printASTFuncDef(ASTFuncDef const *def) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func def\"");

	if (def->typeSpec) {
		n += printf(", \"Type Spec\": ");
		n += printASTTypeSpec(def->typeSpec);
	}

	if (def->funcDecl) {
		n += printf(", \"Declarator\": ");
		n += printASTDeclarator((ASTDeclarator *) def->funcDecl);
	}

	if (def->compoundStm) {
		n += printf(", \"Compound Statement\": ");
		n += printASTCompStm(def->compoundStm);
	}

	n += printf("}");

	return n;
}

int astFuncDefChildCount(ASTFuncDef const *node) {
	return 3;
}

ASTNode *astFuncDefGetChild(ASTFuncDef *node, int index) {
	return (ASTNode *[]) {
		(ASTNode *) node->typeSpec,
		(ASTNode *) node->funcDecl,
		(ASTNode *) node->compoundStm,
	}[index];
}
