#include "intConstant.h"
#include "astUtil.h"
#include <stdio.h>

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
		return _parseDecimal(node, content);
	} else if (content[1] == 'x' || content[1] == 'X') {
		return _parseHex(node, content);
	} else if (content[1] == 'b' || content[1] == 'B') {
		return _parseBin(node, content);
	} else {
		return _parseHex(node, content);
	}
}

int printASTIntContant(const ASTIntConstant *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"IntConstant\"");

	n += printf(", \"value\": %lu", node->value);

	n += printf("}");

	return n;
}
