#include "identifier.h"

#include <string.h>
#include <stdio.h>

#include "node.h"

struct ASTNodeVTable _vtable = {
	{
		(FreeFunc) freeASTIdentifier,
		(PrintFunc) printASTIdentifier,
	},
	(ASTChildCount) astIdentifierChildCount,
	(ASTGetChild) astIdentifierGetChild,
};

void initASTIdentifier(ASTIdentifier *node) {
	initASTNode((ASTNode *) node, node->node.tok, &_vtable);
	node->name = NULL;
	node->node.type = AST_IDENTIFIER;
	initSTypeRef(&node->typeRef);
}

void freeASTIdentifier(ASTIdentifier *node) {
	if (node->name) free(node->name);
	//TODO("free typeref");
}

int parseASTIdentifier(
		ASTIdentifier *node,
		struct Token const *tok,
		struct ASTScope *scope) 
{
	AST_VALID(ASTIdentifier);
	initASTIdentifier(node);
	if (tok->type == TT_IDENTIFIER) {
		node->name = strdup(tok->contents);
		node->node.type = AST_IDENTIFIER;
		return 1;
	} else {
		return 0;
	}
}

int printASTIdentifier(ASTIdentifier const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	n += printf(", \"name\": ");
	n += printJsonStr(node->name);

	n += printf("}");
	
	return n;

}

int astIdentifierChildCount(ASTIdentifier const *node) {
	return 0;
}

ASTNode *astIdentifierGetChild(ASTIdentifier *node, int index) {
	return NULL;
}

