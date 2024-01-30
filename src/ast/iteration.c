#include <stdlib.h>

#include "declaration.h"
#include "iteration.h"
#include "node.h"
#include "operation.h"
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
		freeASTNode(node->statement);
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
		logCerr(CERR_BRACE, tok + n, "Expecting ( after while");
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_INV_EXP, tok + n, "Expecting expression after while");
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expecting ) after while");
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_EXP_STM, tok + n, "Expecting statement after while");
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
		n += printASTNode(node->statement);
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
		freeASTNode(node->statement);
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

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_EXP_STM, tok + n, "Expecting statement after do keyword");
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_WHILE) {
		n++;
	} else {
		logCerr(CERR_WHILE, tok + n, "Expecting while after do statement");
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expecting ( after do while");
		freeASTDoWhile(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_INV_EXP, tok + n, "Expected expression after do while statement");
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expecting ) following do while");
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		logCerr(CERR_MISSING_SEMI, tok + n, "Expecting ; following do while");
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
		n += printASTNode(node->statement);
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
		logCerr(CERR_BRACE, tok + n, "Expecting ( after while");
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);

		if (tok[n].type == TT_SEMI_COLON) {
			n++;
		} else {
			logCerr(CERR_MISSING_SEMI, tok + n, "Expecting ; after equation");
			freeASTFor(node);
			return 0;
		}
	} else if ((res = parseASTDeclaration((ASTDeclaration *) &tempBuf, tok + n, scope))) {
		n += res;
		node->initClause = dupASTNode((ASTNode *) &tempBuf);
	} else if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		logCerr(CERR_INV_EXP, tok + n, "Expecting expression or declaration following (");
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->condExp = dupASTNode((ASTNode *) &tempBuf);
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		logCerr(CERR_MISSING_SEMI, tok + n, "Expecting ; after cond expression");
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTOperation((ASTOperation *) &tempBuf, tok + n, scope))) {
		n += res;
		node->iterExp = dupASTNode((ASTNode *) &tempBuf);
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		logCerr(CERR_BRACE, tok + n, "Expecting ) after iter expression");
		freeASTFor(node);
		return 0;
	}

	if ((res = parseASTStm((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->loopStm = dupASTNode((ASTNode *) &tempBuf);
	} else {
		logCerr(CERR_EXP_STM, tok + n, "Expecting statement after )");
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
