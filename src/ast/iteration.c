#include <stdlib.h>

#include "declaration.h"
#include "iteration.h"
#include "node.h"
#include "expression.h"
#include "statement.h"
#include "astUtil.h"

/************************************************************
 * While
 ************************************************************/

static ASTNodeVTable _whileVTable = {
	{
		(FreeFunc) freeASTWhile,
		(PrintFunc) printASTWhile,
	},
	(ASTChildCount) astWhileChildCount,
	(ASTGetChild) astWhileGetChild,
};

void initASTWhile(ASTWhile *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_whileVTable);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTWhile(ASTWhile *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTWhile(
		ASTWhile *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTWhile(node, tok);
	if (astHasErr()) {
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_WHILE) {
		n++;
	} else {
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("Expecting ( after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting expression after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTStm((ASTStm *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = (ASTStm *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting statement after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	node->node.type = AST_WHILE;
	
	return n;
}

int printASTWhile(const ASTWhile *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"while\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTStm(node->statement);
	}

	n += printf("}");

	return n;
}

int astWhileChildCount(const ASTWhile *node) {
	return 2;
}

ASTNode *astWhileGetChild(ASTWhile *node, int index) {
	return (ASTNode *[]) {
		(ASTNode *) node->expression,
		(ASTNode *) node->statement,
	}[index];
}

/************************************************************
 * Do While
 ************************************************************/

static ASTNodeVTable _doWhileVTable = {
	{
		(FreeFunc) freeASTDoWhile,
		(PrintFunc) printASTDoWhile,
	},
	(ASTChildCount) astDoWhileChildCount,
	(ASTGetChild) astDoWhileGetChild,
};

void initASTDoWhile(ASTDoWhile *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_doWhileVTable);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTDoWhile(ASTDoWhile *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTDoWhile(
		ASTDoWhile *node,
		const struct Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTDoWhile);
	int n = 0, res;
	ASTNodeBuf tempBuf;
	ASTStm tempStm;

	initASTDoWhile(node, tok);
	if (astHasErr()) {
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_DO) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if ((res = parseASTStm(&tempStm, tok + n, scope))) {
		n += res;
		node->statement = malloc(sizeof(ASTStm));
		*node->statement = tempStm;
	} else {
		astErr("Expecting statement after do statement"	, tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_WHILE) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expected expression after do while statement", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) following do while", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		astErr("Expecting ; following do while", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	node->node.type = AST_DO_WHILE;
	return n;
}

int printASTDoWhile(const ASTDoWhile *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"do while\"");

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTStm(node->statement);
	}

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	n += printf("}");

	return n;
}

int astDoWhileChildCount(const ASTDoWhile *node) {
	return 2;
}

ASTNode *astDoWhileGetChild(ASTDoWhile *node, int index) {
	return (ASTNode *[]) {
		(ASTNode *) node->statement,
		(ASTNode *) node->expression,
	}[index];
}

/************************************************************
 * For
 ************************************************************/

static ASTNodeVTable _forVTable = {
	{
		(FreeFunc) freeASTFor,
		(PrintFunc) printASTFor,
	},
	(ASTChildCount) astForChildCount,
	(ASTGetChild) astForGetChild,
};

void initASTFor(ASTFor *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok, &_forVTable);
	node->initClause = NULL;
	node->condExp = NULL;
	node->iterExp = NULL;
	node->loopStm = NULL;
}

void freeASTFor(ASTFor *node) {
	if (node->initClause) {
		freeASTNode(node->initClause);
		free(node->initClause);
	}

	if (node->condExp) {
		freeASTNode(node->condExp);
		free(node->condExp);
	}

	if (node->iterExp) {
		freeASTNode(node->iterExp);
		free(node->iterExp);
	}

	if (node->loopStm) {
		freeASTNode(node->loopStm);
		free(node->loopStm);
	}
}

int parseASTFor(
		ASTFor *node,
		const Token *tok,
		struct ASTScope *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFor(node, tok);
	if (astHasErr()) {
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_FOR) {
		n++;
	} else {
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("Expecting ( after while", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);

		if (tok[n].type == TT_SEMI_COLON) {
			n++;
		} else {
			astErr("Expecting ; after equation", tok + n);
			freeASTFor(node);
			return 0;
		}
	} else if ((res = parseASTDeclarator((ASTDeclarator *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting expression or declaration following (", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->condExp = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting equation after ;", tok + n);
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		astErr("Expecting ; after cond expression", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->iterExp = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting iter expression after ;", tok + n);
		freeASTFor(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) after iter expression", tok + n);
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTStm((ASTStm *) &tempBuf, tok + n, scope))) {
		n += res;
		node->loopStm = dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting statement after )", tok + n);
		freeASTFor(node);
		return 0;
	}

	node->node.type = AST_FOR;
	return n;
}

int printASTFor(ASTFor const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));

	if (node->initClause) {
		n += printf(", \"init clause\": ");
		n += printASTNode(node->initClause);
	}

	if (node->condExp) {
		n += printf(", \"condition expression\": ");
		n += printASTNode(node->condExp);
	}

	if (node->iterExp) {
		n += printf(", \"iteration expression\": ");
		n += printASTNode(node->iterExp);
	}

	if (node->loopStm) {
		n += printf(", \"loop statement\": ");
		n += printASTNode(node->loopStm);
	}

	n += printf("}");

	return n;
}

int astForChildCount(const ASTFor *node) {
	return 4;
}

ASTNode *astForGetChild(ASTFor *node, int index) {
	return (ASTNode *[]) {
		node->initClause,
		node->condExp,
		node->iterExp,
		node->loopStm,
	}[index];
}
