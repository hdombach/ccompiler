#include <stdlib.h>

#include "specialDecl.h"
#include "param.h"
#include "declaration.h"
#include "expression.h"
#include "astUtil.h"
#include "node.h"
#include "../sem/scope.h"


/************************************************************
 * Array Decl
 ************************************************************/

void initASTArrayDecl(ASTArrayDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	decl->exp = NULL;
	decl->encl = NULL;
}

void freeASTArrayDecl(ASTArrayDecl *decl) {
	if (decl->exp) {
		freeASTNode(decl->exp);
		free(decl->exp);
	}
	if (decl->encl) {
		freeASTNode(decl->encl);
		free(decl->encl);
	}
}

int parseASTArrayDecl(
		ASTArrayDecl *decl,
		const Token *tok,
		ASTNode *encl,
		struct ASTScope *scope) {
	AST_VALID(ASTArrayDecl);
	int n = 0, res;

	initASTArrayDecl(decl, tok);
	decl->node.tok = tok;
	if (encl) {
		decl->encl = encl;
	}
	if (astHasErr()) {
		freeASTArrayDecl(decl);
		return 0;
	}
	
	if (tok[n].type == TT_O_BRACE) {
		n++;
	} else {
		freeASTArrayDecl(decl);
		return 0;
	}

	ASTNodeBuf tempBuf;

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		decl->exp = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	}

	if (tok[n].type == TT_C_BRACE) {
		n++;
	} else {
		astErr("Expecting ] at end of array declaration", tok + n);
		freeASTArrayDecl(decl);
		return 0;
	}

	decl->node.type = AST_ARRAY_DECL;

	return n;
}

int printASTArrayDecl(const ASTArrayDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"Node Type\": \"ArrayDecl\"");

	if (decl->exp) {
		n += printf(", \"size\": ");
		n += printASTNode(decl->exp);
	}

	if (decl->encl) {
		n += printf(", \"enclosing type\": ");
		n += printASTNode(decl->encl);
	}

	n += printf("}");

	return n;
}

int astArrayDeclChildCount(const ASTArrayDecl *node) {
	return 2;
}

ASTNode *astArrayDeclGetChild(ASTArrayDecl *node, int index) {
	return (ASTNode *[]) {
		node->exp,
		node->encl,
	}[index];
}

/************************************************************
 * Enum Decl
 ************************************************************/

void initASTEnumeratorDecl(ASTEnumeratorDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	decl->name = NULL;
	decl->exp = NULL;
}

void freeASTEnumeratorDecl(ASTEnumeratorDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	if (decl->exp) {
		freeASTNode(decl->exp);
		free(decl->exp);
	}
}

int parseASTEnumeratorDecl(
		ASTEnumeratorDecl *decl,
		Token const *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTEnumeratorDecl);
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}

	initASTEnumeratorDecl(decl, tok);

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	} else {
		freeASTEnumeratorDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_EQL) {
		n++;
	} else {
		decl->node.type = AST_ENUMERATOR_DECL;
		return n;
	}

	decl->exp = malloc(AST_NODE_S);
	if ((res = parseASTExp(decl->exp, tok + n, scope))) {
		n += res;
	} else {
		astErr("Expected expression after enumerator", tok + n);
		free(decl->exp);
		decl->exp = NULL;
		freeASTEnumeratorDecl(decl);
		return 0;
	}

	decl->node.type = AST_ENUMERATOR_DECL;
	return n;
}

int printASTEnumeratorDecl(const ASTEnumeratorDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"Enumerator declaration\"");

	n += printf(", \"Enumerator name\": ");
	n += printJsonStr(decl->name);

	if (decl->exp) {
		n += printf(", \"Enumerator value\": ");
		n += printASTNode(decl->exp);
	}

	n += printf("}");

	return n;
}

int astEnumeratorDeclChildCount(ASTEnumeratorDecl const *node) {
	return 1;
}

ASTNode *astEnumeratorDeclGetChild(ASTEnumeratorDecl *node, int index) {
	return (ASTNode *[]) {
		node->exp,
	}[index];
}

void initASTEnumDecl(ASTEnumDecl *decl, Token const *tok) {
	decl->name = NULL;
	initDListEmpty(&decl->enumerators, AST_NODE_S);
	initASTNode((ASTNode *) decl, tok);
}

void freeASTEnumDecl(ASTEnumDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	freeDList(&decl->enumerators, (FreeFunc) freeASTNode);
}

int parseASTEnumDecl(
		ASTEnumDecl *decl,
		Token const *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTEnumDecl);
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}
	
	initASTEnumDecl(decl, tok);

	if (tok[n].type == TT_ENUM) {
		n++;
	} else {
		freeASTEnumDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	}

	if (tok[n].type == TT_O_CURLY) {
		n++;
	} else {
		decl->node.type = AST_ENUM_DECL;
		return n;
	}

	while (1) {
		ASTNodeBuf tempBuf;
		if ((res = parseASTEnumeratorDecl((ASTEnumeratorDecl *) &tempBuf, tok + n, scope))) {
			n += res;
			dlistApp(&decl->enumerators, &tempBuf);
		} else {
			break;
		}

		if (tok[n].type == TT_COMMA) {
			n++;
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		astErr("Expecting }", tok + n);
		freeASTEnumDecl(decl);
		return 0;
	}

	decl->node.type = AST_ENUM_DECL;
	return n;
}

int printASTEnumDecl(const ASTEnumDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"enumeration\"");

	if (decl->name) {
		n += printf(", \"enum name\": ");
		n += printJsonStr(decl->name);
	}

	n += printf(", \"Enumerators\": ");
	n += printDList(&decl->enumerators, (PrintFunc) printASTEnumeratorDecl);

	n += printf("}");

	return n;
}

int astEnumDeclChildCount(ASTEnumDecl const *node) {
	return node->enumerators.size;
}

ASTNode *astEnumDeclGetChild(ASTEnumDecl *node, int index) {
	return dlistGetm(&node->enumerators, index);
}

/************************************************************
 * Struct Decl
 ************************************************************/

void initASTStructDecl(ASTStructDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	decl->name = NULL;
	initDListEmpty(&decl->items, AST_NODE_S);
	decl->scope = malloc(sizeof(struct ASTScope));
	initASTScope(decl->scope);
	decl->isUnion = 0;
}

void freeASTStructDecl(ASTStructDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	freeDList(&decl->items, (FreeFunc) freeASTNode);
	if (decl->scope) {
		freeASTScope(decl->scope);
		free(decl->scope);
		decl->scope = NULL;
	}
}

int parseASTStructDecl(
		ASTStructDecl *decl,
		const Token *tok,
		ASTScope *scope)
{
	AST_VALID(ASTStructDecl);
	int res, n = 0;

	decl->node.type = AST_UNKNOWN;
	if (astHasErr()) {
		return 0;
	}

	initASTStructDecl(decl, tok);
	decl->scope->parent = (ASTScope *) scope; //is const in this context

	if (tok[n].type == TT_STRUCT) {
		n++;
	} else if (tok[n].type == TT_UNION) {
		decl->isUnion = 1;
		n++;
	} else {
		freeASTStructDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	}

	if (tok[n].type == TT_O_CURLY) {
		n++;
	} else {
		decl->node.type = AST_STRUCT_DECL;
		return n;
	}

	ASTNodeBuf tempBuf;
	while (1) {
		if ((res = parseASTDeclaration((ASTDeclaration *) &tempBuf, tok + n, decl->scope))) {
			n += res;
			dlistApp(&decl->items, &tempBuf);
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		astErr("Expecting }", tok + n);
		freeASTStructDecl(decl);
		return 0;
	}

	decl->node.type = AST_STRUCT_DECL;
	return n;
}

int printASTStructDecl(const ASTStructDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"struct decl\"");

	if (decl->name) {
		if (decl->isUnion) {
			n += printf(", \"union name\": ");
		} else {
			n += printf(", \"struct name\": ");
		}
		n += printJsonStr(decl->name);
	}

	if (decl->isUnion) {
		n += printf(", \"union declarations\": ");
	} else {
		n += printf(", \"struct declarations\": ");
	}
	n += printDList(&decl->items, (PrintFunc) printASTNode);

	n += printf("}");

	return n;
}

int astStructDeclChildCount(ASTStructDecl const *node) {
	return node->items.size;
}

ASTNode *astStructDeclGetChild(ASTStructDecl *node, int index) {
	return dlistGetm(&node->items, index);
}

/************************************************************
 * Func Decl
 ************************************************************/

void initASTFuncDecl(ASTFuncDecl *decl, Token const *tok) {
	initASTNode((ASTNode *) decl, tok);
	initDListEmpty(&decl->params, AST_NODE_S);
	decl->scope = malloc(sizeof(ASTScope));
	initASTScope(decl->scope);
	decl->encl = NULL;
	decl->hasEllipses = 0;
}

void freeASTFuncDecl(ASTFuncDecl *decl) {
	freeDList(&decl->params, (FreeFunc) freeASTParam);
	freeASTScope(decl->scope);
	free(decl->scope);
	if (decl->encl) {
		freeASTNode(decl->encl);
		free(decl->encl);
	}
}

int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		ASTNode *encl,
		struct ASTScope *scope)
{
	AST_VALID(ASTFuncDecl);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFuncDecl(decl, tok);
	if (encl) {
		decl->encl = encl;
	}
	if (astHasErr()) {
		freeASTFuncDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTFuncDecl(decl);
		return 0;
	}

	if (tok[n].type != TT_C_PARAN) {
		while (1) {
			if ((res = parseASTParam((ASTParam *) &tempBuf, tok + n, scope))) {
				dlistApp(&decl->params, &tempBuf);
				n += res;
			} else if (tok[n].type == TT_DOTS) {
				decl->hasEllipses = 1;
				n++;
			} else {
				freeASTFuncDecl(decl);
				return 0;
			}

			if (tok[n].type == TT_COMMA) {
				n++;
			} else {
				break;
			}
		}
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting )", tok + n);
		freeASTFuncDecl(decl);
		return 0;
	}

	decl->node.type = AST_FUNC_DECL;
	return n;
}

int printASTFuncDecl(const ASTFuncDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"func decl\"");

	if (decl->params.size) {
		n += printf(", \"params\": ");
		printDList(&decl->params, (PrintFunc) printASTParam);
	}

	if (decl->hasEllipses) {
		n += printf(", \"Has ellipses\": true");
	}

	if (decl->encl) {
		n += printf(", \"enclosing type\": ");
		n += printASTNode(decl->encl);
	}

	n += printf("}");

	return n;
}

int astFuncDeclChildCount(const ASTFuncDecl *node) {
	return node->params.size + 1;
}

ASTNode *astFuncDeclGetChild(ASTFuncDecl *node, int index) {
	if (index < node->params.size) {
		return dlistGetm(&node->params, index);
	} else {
		return node->encl;
	}
}
