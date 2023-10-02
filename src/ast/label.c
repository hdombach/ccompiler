#include <stdlib.h>
#include <string.h>

#include "label.h"
#include "expression.h"
#include "astUtil.h"
#include "expression.h"
#include "../util/util.h"
#include "node.h"

void initASTLabel(ASTLabel *label) {
	label->type = AST_LT_UNKNOWN;
}

void freeASTLabel(ASTLabel *label) {
	switch (label->type) {
		case AST_LT_IDENTIFIER:
			free(label->c.identifier);
			break;
		case AST_LT_CASE:
			freeASTExp(label->c.expression);
			break;
		default:
			break;
	}
	label->type = AST_LT_UNKNOWN;
}

int parseASTLabel(
		ASTLabel *label,
		struct Token const *tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	char *tempIdentifier;
	ASTNodeBuf tempBuf;
	ASTNode *tempNode = (ASTNode *) &tempBuf;

	initASTLabel(label);
	if (astHasErr()) {
		freeASTLabel(label);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		tempIdentifier = tok[n].contents;
		n++;
		if (tok[n].type == TT_COLON) {
			n++;
		} else {
			freeASTLabel(label);
			return 0;
		}
		label->type = AST_LT_IDENTIFIER;
		label->c.identifier = strdup(tempIdentifier);

	} else if (tok[n].type == TT_CASE) {
		n++;

		if ((res = parseASTExp(tempNode, tok + n, scope))) {
			n += res;
		} else {
			freeASTLabel(label);
			astErr("Expected expression following case", tok + n);
			return 0;
		}

		if (tok[n].type == TT_COLON) {
			n++;
		} else {
			freeASTLabel(label);
			astErr("Expecting : following case", tok + n);
			return 0;
		}

		label->type = AST_LT_CASE;
		label->c.expression = malloc(AST_NODE_S);
		mvASTNode(label->c.expression, tempNode);
	} else if (tok[n].type == TT_DEFAULT) {
		n++;

		if (tok[n].type == TT_COLON) {
			n++;
		} else {
			freeASTLabel(label);
			astErr("expecting : following default", tok + n);
			return 0;
		}
		label->type = AST_LT_DEFAULT;
	}
	return n;
}

int printASTLabel(ASTLabel const *label) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"label\"");

	n += printf(", \"label type\": ");
	switch(label->type) {
		case AST_LT_DEFAULT:
			n += printf("\"default\"");
			break;
		case AST_LT_IDENTIFIER:
			n += printf("\"identifier\"");

			n += printf(", \"name\": ");
			n += printJsonStr(label->c.identifier);
			break;
		case AST_LT_CASE:
			n += printf("\"case\"");

			n += printf(", \"expression\": ");
			n += printASTExp(label->c.expression);
			break;
		default:
			n += printf("\"unknown\"");
	}

	n += printf("}");

	return n;
}
