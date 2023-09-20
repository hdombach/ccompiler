#include <stdlib.h>
#include <string.h>

#include "enumDecl.h"
#include "expression.h"
#include "astUtil.h"
#include "../util/util.h"

void initASTEnumeratorDecl(ASTEnumeratorDecl *decl) {
	decl->name = NULL;
	decl->exp = NULL;
}

void freeASTEnumeratorDecl(ASTEnumeratorDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	if (decl->exp) {
		freeASTExp(decl->exp);
		free(decl->exp);
	}
}

int parseASTEnumeratorDecl(ASTEnumeratorDecl *decl, Token const *tok) {
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}

	initASTEnumeratorDecl(decl);

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	} else {
		freeASTEnumeratorDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_EQL) {
		n++;
	} else {
		return n;
	}

	decl->exp = malloc(sizeof(ASTExp));
	if ((res = parseASTExp(decl->exp, tok + n))) {
		n += res;
	} else {
		astErr("Expected expression after enumerator", tok + n);
		free(decl->exp);
		decl->exp = NULL;
		freeASTEnumeratorDecl(decl);
		return 0;
	}

	return n;
}

int printASTEnumeratorDecl(const ASTEnumeratorDecl *decl) {
	int n = 0;

	n += printf("{");

	n += printf("\"Enumerator name\": ");
	n += printJsonStr(decl->name);

	if (decl->exp) {
		n += printf(", \"Enumerator value\": ");
		n += printASTExp(decl->exp);
	}

	n += printf("}");

	return n;
}

void initASTEnumDecl(ASTEnumDecl *decl) {
	decl->name = NULL;
	initDList(&decl->enumerators, sizeof(ASTEnumeratorDecl));
}

void freeASTEnumDecl(ASTEnumDecl *decl) {
	if (decl->name) {
		free(decl->name);
	}
	freeDList(&decl->enumerators, (FreeFunc) freeASTEnumeratorDecl);
}

int parseASTEnumDecl(ASTEnumDecl *decl, Token const *tok) {
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}
	
	initASTEnumDecl(decl);

	if (tok[n].type == TT_ENUM) {
		n++;
	} else {
		freeASTEnumDecl(decl);
		return 0;
	}

	if (tok[n].type == TT_IDENTIFIER) {
		decl->name = strdup(tok[n].contents);
		n++;
	}

	if (tok[n].type == TT_O_CURLY) {
		n++;
	} else {
		astErr("Expecting {", tok + n);
		freeASTEnumDecl(decl);
		return 0;
	}

	while (1) {
		ASTEnumeratorDecl enumerator;
		if ((res = parseASTEnumeratorDecl(&enumerator, tok + n))) {
			n += res;
			dlistApp(&decl->enumerators, &enumerator);
		} else {
			break;
		}

		if (tok[n].type == TT_COMMA) {
			n++;
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		astErr("Expecting }", tok + n);
		freeASTEnumDecl(decl);
		return 0;
	}

	return n;
}

int printASTEnumDecl(const ASTEnumDecl *decl) {
	int n = 0, isFirst = 1;

	n += printf("{");

	if (decl->name) {
		n += printf("\"enum name\": ");
		n += printJsonStr(decl->name);
		isFirst = 0;
	}

	if (isFirst) {
		isFirst = 0;
	} else {
		n += printf(", ");
	}
	n += printf("\"Enumerators\": ");
	n += printDList(&decl->enumerators, (PrintFunc) printASTEnumeratorDecl);

	n += printf("}");

	return n;
}