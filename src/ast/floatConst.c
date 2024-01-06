#include "floatConst.h"
#include "astUtil.h"
#include "node.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>

static ASTNodeVTable _vtable = {
	{
		NULL,
		(PrintFunc) printASTFloatConstant,
	},
	(ASTChildCount) astFloatConstantChildCount,
	(ASTGetChild) astFloatConstantGetChild,
};

void initASTFloatConstant(ASTFloatConstant *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_vtable);
}

/**
 * @brief Convert ascii to int
 * @param[in] c ASCII digit to convert
 * @param[out] result The parsed digit
 * @returns The number of parsed characters
 */
static int _atoi(const char *c, unsigned long int *result) {
	int ret = 0;
	*result = 0;
	while (isdigit(*c)) {
		*result = *result * 10 + *c - '0';
		ret++;
		c++;
	}
	return ret;
}

/**
 * @brief Convert ascii to fractional part
 */
static int _atof(const char *c, double *result) {
	int ret = 0;
	*result = 0;
	while (isdigit(*c)) {
		*result = (*result + *c - '0') * 0.1;
		c++;
		ret++;
	}
	return ret;
}

int parseASTFloatConstant(ASTFloatConstant *node, const Token *tok) {
	int n = 0;

	AST_VALID(ASTFloatConstant);
	if (astHasErr()) return 0;

	initASTFloatConstant(node, tok);

	if (tok->type != TT_NUMB_CONSTANT) return 0;

	char const *content = tok->contents;

	if (!*content) return 0;

	double fracPart = 0;
	unsigned long int wholePart = 0;
	unsigned long int expPart = 0;
	double value = 0;
	int isNegExp = 0;

	content += _atoi(content, &wholePart);
	if (*content == '.') {
		content++;
		content += _atof(content, &fracPart);
	}
	if (*content == 'e' || *content == 'E') {
		content++;
		if (*content == '+') {
			content++;
		} else if (*content == '-') {
			isNegExp = 1;
			content++;
		}
		content += _atoi(content, &expPart);
	}

	value = wholePart + fracPart;
	if (isNegExp) {
		value *= pow(0.1, expPart);
	} else {
		value *= pow(10, expPart);
	}

	node->valueType = FCT_DOUBLE;
	if (*content == 'f' || *content == 'F') {
		node->valueType = FCT_FLOAT;
		content++;
	} else if (*content == 'l' || *content == 'L') {
		node->valueType = FCT_DOUBLE;
		content++;
	} else {
		node->valueType = FCT_DOUBLE;
	}

	if (*content) return 0;

	if (node->valueType == FCT_DOUBLE) {
		node->value.d = value;
	} else {
		node->value.f = value;
	}

	node->node.type = AST_FLOAT_CONSTANT;
	return 1;
}

int printASTFloatConstant(const ASTFloatConstant *node) {
	int n = 0;
	
	n += printf("{");
	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));
	if (node->valueType == FCT_DOUBLE) {
		n += printf(", \"value\": %f", node->value.d);
	} else {
		n += printf(", \"value\": %f", node->value.f);
	}
	n += printf("}");

	return n;
}

int astFloatConstantChildCount(const ASTFloatConstant *node) {
	return 0;
}

ASTNode *astFloatConstantGetChild(ASTFloatConstant *node, int index) {
	return NULL;
}
