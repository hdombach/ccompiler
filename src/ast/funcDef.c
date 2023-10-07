#include "funcDef.h"
#include "../util/dlist.h"
#include "astUtil.h"
#include "compStatement.h"
#include "declaration.h"
#include "funcDecl.h"
#include "param.h"
#include <stdio.h>

void initASTFuncDef(ASTFuncDef *def) {
	initASTTypeSpec(&def->typeSpec);
	initASTDeclarator(&def->funcDecl);
	initASTCompStm(&def->compoundStm);
}

void freeASTFuncDef(ASTFuncDef *def) {
	freeASTTypeSpec(&def->typeSpec);
	freeASTDeclarator(&def->funcDecl);
	freeASTCompStm(&def->compoundStm);
}

int parseASTFuncDef(
		ASTFuncDef *def,
		Token const * tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTDeclarator *curDecl, *prevDecl;

	initASTFuncDef(def);

	if (astHasErr()) {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTTypeSpec(&def->typeSpec, tok + n, scope))) {
		n += res;
	} else {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTDeclarator(&def->funcDecl, tok + n, scope))) {
		n += res;
	} else {
		freeASTFuncDef(def);
		return 0;
	}

	ASTDeclarator *prevPrev = NULL;
	prevDecl = NULL;
	curDecl = &def->funcDecl;
	while (curDecl) {
		prevPrev = prevDecl;
		prevDecl = curDecl;
		switch (curDecl->type) {
			case AST_DT_POINTER:
				curDecl = curDecl->c.pointer;
				break;
			case AST_DT_ARRAY:
				curDecl = ((ASTArrayDecl *) curDecl->c.array)->encl;
				break;
			case AST_DT_FUNC:
				curDecl = curDecl->c.func->encl;
				if (curDecl && curDecl->type == AST_DT_IDENTIFIER) {
					curDecl = NULL;
				}
				break;
			default:
				curDecl = NULL;
				break;
		}
	}


	if (!prevDecl || prevDecl->type != AST_DT_FUNC) {
		freeASTFuncDef(def);
		return 0;
	}

	if ((res = parseASTCompStm(&def->compoundStm, tok + n, scope))) {
		//printf("parsing func def\n");
		n += res;
	} else {
		//Don't throw error
		freeASTFuncDef(def);
		return 0;
	}

	return n;
}

int printASTFuncDef(ASTFuncDef const *def) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func def\"");

	n += printf(", \"Type Spec\": ");
	n += printASTTypeSpec(&def->typeSpec);

	n += printf(", \"Declarator\": ");
	n += printASTDeclarator(&def->funcDecl);

	n += printf(", \"Compound Statement\": ");
	n += printASTCompStm(&def->compoundStm);

	n += printf("}");

	return n;
}
