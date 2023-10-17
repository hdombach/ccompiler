#include <stdlib.h>
#include <string.h>

#include "label.h"
#include "expression.h"
#include "astUtil.h"
#include "expression.h"
#include "../util/util.h"
#include "node.h"

void initASTLblIdentifier(ASTLblIdentifier *node) {
	initASTNode((ASTNode *) node);
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
		struct ASTScope const *scope)
{
	int n = 0, res;
	char *tempIdentifier;
	initASTLblIdentifier(node);
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

void initASTLblCase(ASTLblCase *node) {
	initASTNode((ASTNode *) node);
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
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTLblCase(node);
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

int parseASTLblDefault(
		ASTNode *node,
		const struct Token *tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTNode(node);
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
		struct ASTScope const *scope)
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
