#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "label.h"
#include "astUtil.h"
#include "node.h"
#include "../util/log.h"
#include "operation.h"
#include "statement.h"

int parseASTLbl(
		ASTNode *node,
		struct Token const *tok,
		struct ASTScope *scope)
{
	int res;

	if ((res = parseASTLblIdentifier((ASTLblIdentifier *) node, tok, scope))) {
		return res;
	} else if ((res = parseASTLblCase((ASTLblCase *) node, tok, scope))) {
		return res;
	} else if ((res = parseASTLblDefault((ASTLblDefault *) node, tok, scope))) {
		return res;
	}
	return 0;
}

static ASTNodeVTable _lblIdentifierVTable = {
	{
		(FreeFunc) freeASTLblIdentifier,
		(PrintFunc) printASTLblIdentifier,
	},
	(ASTChildCount) astLblIdentifierChildCount,
	(ASTGetChild) astLblIdentifierGetChild,
};

void initASTLblIdentifier(ASTLblIdentifier *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_lblIdentifierVTable);
	node->name = NULL;
	node->stm = NULL;
}

void freeASTLblIdentifier(ASTLblIdentifier *node) {
	if (node->name) {
		free(node->name);
		node->name = NULL;
	}
	if (node->stm) {
		freeASTNode(node->stm);
		node->stm = NULL;
	}
}

int parseASTLblIdentifier(
		ASTLblIdentifier *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTLblIdentifier(node, tok);
	if (astHasErr()) {
		freeASTLblIdentifier(node);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		node->name = strdup(tok[n].contents);
		n++;
	} else {
		freeASTLblIdentifier(node);
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTLblIdentifier(node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->stm = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTLblIdentifier(node);
		return 0;
	}

	node->node.type = AST_LBL_IDENTIFIER;
	return n;
}

int printASTLblIdentifier(const ASTLblIdentifier *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	n += printf(", \"identifier\": \"%s\"", node->name);

	n += printf(", \"statement\": ");

	n += printASTNode(node->stm);

	n += printf("}");

	return n;
}

int astLblIdentifierChildCount(const ASTLblIdentifier *node) {
	return 1;
}

ASTNode *astLblIdentifierGetChild(ASTLblIdentifier *node, int index) {
	return node->stm;
}

static ASTNodeVTable _lblCaseVTable = {
	{
		(FreeFunc) freeASTLblCase,
		(PrintFunc) printASTLblCase,
	},
	(ASTChildCount) astLblCaseChildCount,
	(ASTGetChild) astLblCaseGetChild,
};

void initASTLblCase(ASTLblCase *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_lblCaseVTable);
	node->expression = NULL;
}

void freeASTLblCase(ASTLblCase *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
		node->expression = NULL;
	}

	if (node->stm) {
		freeASTNode(node->stm);
		free(node->stm);
		node->stm = NULL;
	}
}

int parseASTLblCase(
		ASTLblCase *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTLblCase(node, tok);
	if (astHasErr()) {
		freeASTLblCase(node);
		return 0;
	}

	if (tok[n].type == TT_CASE) {
		n++;
	} else {
		freeASTLblCase(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		node->expression = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTLblCase(node);
		logCerr(CERR_INV_EXP, tok + n, "Expecting expression following case");
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTLblCase(node);
		logCerr(CERR_TOK, tok + n, "Expecting : following case");
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		node->stm = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTLblCase(node);
		return 0;
	}

	node->node.type = AST_LBL_CASE;

	return n;
}

int printASTLblCase(const ASTLblCase *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->stm) {
		n += printf(", \"statement\": ");
		n += printASTNode(node->stm);
	}

	n += printf("}");

	return n;
}

int astLblCaseChildCount(ASTLblCase const *node) {
	return 2;
}

ASTNode *astLblCaseGetChild(ASTLblCase *node, int index) {
	return (ASTNode *[]){
		(ASTNode *) node->expression,
		(ASTNode *) node->stm,
	}[index];
}

static ASTNodeVTable _defaultVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTLblDefault,
	},
	(ASTChildCount) astLblDefaultChildCount,
	(ASTGetChild) astLblDefaultGetChild,
};

void initASTLblDefault(ASTLblDefault *node, const Token *tok) {
	initASTNode((ASTNode *) node, tok, &_defaultVTable);
	node->stm = NULL;
}

void freeASTLblDefault(ASTLblDefault *node, const Token *tok) {
	if (node->stm) {
		freeASTNode(node->stm);
		free(node->stm);
		node->stm = NULL;
	}
}

int parseASTLblDefault(
		ASTLblDefault *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTNode((ASTNode *) node, tok, &_defaultVTable);
	if (astHasErr()) {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	if (tok[n].type == TT_DEFAULT) {
		n++;
	} else {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->stm = dupASTNode((ASTNode *) &tempBuf);
	} else {
		freeASTNode((ASTNode *) node);
		return 0;
	}

	node->node.type = AST_LBL_DEFAULT;

	return n;
}

int printASTLblDefault(ASTLblDefault const *node) {
	return printf("\"%s\"", astNodeTypeStr(node->node.type));
}

int astLblDefaultChildCount(ASTLblDefault const *node) {
	return 1;
}

ASTNode *astLblDefaultGetChild(ASTLblDefault *node, int index) {
	return node->stm;
}
