#include <stdio.h>

#include "intConstant.h"
#include "astUtil.h"
#include "node.h"

int _parseDecimal(ASTIntConstant *node, char const *content) {
	while (*content) {
		if ('0' > *content || '9' < *content) {
			return 0;
		}
		node->value = node->value * 10 + (*content - '0');
		content++;
	}
	return 1;
}

int _parseOctal(ASTIntConstant *node, char const *content) {
	while (*content) {
		if ('0' > *content || '7' < *content) {
			return 0;
		}
		node->value = node->value * 8 + (*content - '0');
		content++;
	}
	return 1;
}

int _parseHex(ASTIntConstant *node, char const *content) {
	while (*content) {
		if ('0' <= *content && '9' >= *content) {
			node->value = node->value * 16 + (*content - '0');
		} else if ('a' <= *content && 'f' >= *content) {
			node->value = node->value * 16 + (*content - 'a' + 10);
		} else if ('A' <= *content && 'F' >= *content) {
			node->value = node->value * 16 + (*content - 'A' + 10);
		} else {
			return 0;
		}
		content++;
	}
	return 1;
}

int _parseBin(ASTIntConstant *node, char const *content) {
	while (*content) {
		if (*content < '0' || *content > '1') {
			return 0;
		}
		node->value = node->value * 2 + (*content - '0');;
		content++;
	}
	return 1;
}

int parseASTIntConstant(ASTIntConstant *node, const Token *tok) {
	int n = 0;

	AST_VALID(ASTIntConstant);
	if (astHasErr()) {
		return 0;
	}

	if (tok->type != TT_NUMB_CONSTANT) {
		return 0;
	}

	char const *content = tok->contents;
	node->value = 0;

	if (!*content) {
		return 0;
	}
	if (content[0] != 0) {
		n = _parseDecimal(node, content);
	} else if (content[1] == 'x' || content[1] == 'X') {
		n = _parseHex(node, content);
	} else if (content[1] == 'b' || content[1] == 'B') {
		n = _parseBin(node, content);
	} else {
		n = _parseHex(node, content);
	}

	if (n) {
		node->node.type = AST_INT_CONSTANT;
	}

	return n;
}

int printASTIntContant(const ASTIntConstant *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"IntConstant\"");

	n += printf(", \"value\": %lu", node->value);

	n += printf("}");

	return n;
}

int astIntConstantChildCount(const ASTIntConstant *node) {
	return 0;
}

ASTNode *astIntConstantGetChild(ASTIntConstant *node, int index) {
	return NULL;
}
