#include <stdlib.h>
#include <string.h>

#include "enumDecl.h"
#include "expression.h"
#include "astUtil.h"
#include "../util/util.h"
#include "node.h"

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
		struct ASTScope const *scope)
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

ASTTravRes astEnumeratorDeclTrav(
		ASTEnumeratorDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	if (node->exp) {
		result = astNodeTrav(node->exp, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
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
		struct ASTScope const *scope)
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

ASTTravRes astEnumDeclTrav(ASTEnumDecl *decl,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	for (int i = 0; i < decl->enumerators.size; i++) {
		ASTNode *node = dlistGetm(&decl->enumerators, i);
		result = astNodeTrav(node, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}
