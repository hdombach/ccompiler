#include <stdlib.h>
#include <string.h>

#include "label.h"
#include "expression.h"
#include "astUtil.h"
#include "expression.h"
#include "node.h"

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
}

void freeASTLblIdentifier(ASTLblIdentifier *node) {
	if (node->name) {
		free(node->name);
	}
}

int parseASTLblIdentifier(
		ASTLblIdentifier *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	char *tempIdentifier;
	initASTLblIdentifier(node, tok);
	if (astHasErr()) {
		freeASTLblIdentifier(node);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		tempIdentifier = tok[n].contents;
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
	node->name = strdup(tempIdentifier);
	node->node.type = AST_LBL_IDENTIFIER;
	return n;
}

int printASTLblIdentifier(const ASTLblIdentifier *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	n += printf(", \"identifier\": \"%s\"", node->name);

	n += printf("}");

	return n;
}

int astLblIdentifierChildCount(const ASTLblIdentifier *node) {
	return 0;
}

ASTNode *astLblIdentifierGetChild(ASTLblIdentifier *node, int index) {
	return NULL;
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

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		node->expression = dupASTNode((ASTNode *) &tempBuf);
		n += res;
	} else {
		freeASTLblCase(node);
		astErr("Expected expression following case", tok + n);
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTLblCase(node);
		astErr("Expecting : following case", tok + n);
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

	n += printf("}");

	return n;
}

int astLblCaseChildCount(ASTLblCase const *node) {
	return 1;
}

ASTNode *astLblCaseGetChild(ASTLblCase *node, int index) {
	return node->expression;
}

static ASTNodeVTable _defaultVTable = {
	{
		(FreeFunc) NULL,
		(PrintFunc) printASTLblDefault,
	},
	(ASTChildCount) astLblDefaultChildCount,
	(ASTGetChild) astLblDefaultGetChild,
};

int parseASTLblDefault(
		ASTNode *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTNode(node, tok, &_defaultVTable);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if (tok[n].type == TT_DEFAULT) {
		n++;
	} else {
		freeASTNode(node);
		return 0;
	}

	if (tok[n].type == TT_COLON) {
		n++;
	} else {
		freeASTNode(node);
		return 0;
	}

	node->type = AST_LBL_DEFAULT;

	return n;
}

int printASTLblDefault(const ASTNode *node) {
	return printf("\"%s\"", astNodeTypeStr(node->type));
}

int parseASTLabel(
		ASTNode *node,
		struct Token const *tok,
		struct ASTScope *scope)
{
	int res;

	if ((res = parseASTLblIdentifier((ASTLblIdentifier *) node, tok, scope))) {
		return res;
	} else if ((res = parseASTLblCase((ASTLblCase *) node, tok, scope))) {
		return res;
	} else if ((res = parseASTLblDefault(node, tok, scope))) {
		return res;
	}
	return 0;
}

int astLblDefaultChildCount(ASTNode const *node) {
	return 0;
}

ASTNode *astLblDefaultGetChild(ASTNode *node, int index) {
	return NULL;
}
