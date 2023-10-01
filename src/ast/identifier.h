#pragma once

#include <string.h>
#include <stdio.h>

#include "node.h"
#include "../token.h"
#include "../util/util.h"

struct Token;
struct ASTScope;

typedef struct ASTIdentifier {
	ASTNode node;
	char *name;
} ASTIdentifier;

static void initASTIdentifier(ASTIdentifier *node) {
	node->name = NULL;
	node->node.type = AST_IDENTIFIER;
}

static int parseASTIdentifier(
		ASTIdentifier *node,
		struct Token const *tok,
		struct ASTScope const *scope) 
{
	AST_VALID(ASTIdentifier);
	if (tok->type == TT_IDENTIFIER) {
		node->name = strdup(tok->contents);
		node->node.type = AST_IDENTIFIER;
		return 1;
	} else return 0;
}

static int printASTIdentifier(ASTIdentifier const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	n += printf(", \"name\": ");
	n += printJsonStr(node->name);

	n += printf("}");
	
	return n;
}