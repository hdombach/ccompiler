#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astUtil.h"
#include "declaration.h"

void initASTTypeQualifier(ASTTypeQualifier *qualifiers) {
	*qualifiers = AST_TQ_NONE;
}

//TODO: implimenet warning when there are duplicate things

int parseASTTypeQualifier(ASTTypeQualifier *qualifiers, const Token *tok) {
	if (astErrMsg) {
		return 0;
	}

	switch (tok->type) {
		case TT_CONST:
			*qualifiers |= AST_TQ_CONST;
			return 1;
		case TT_VOLATILE:
			*qualifiers |= AST_TQ_VOLATILE;
			return 1;
		default:
			return 0;
	}
}

int printASTTypeQualifier(const ASTTypeQualifier *qualifiers) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*qualifiers & AST_TQ_CONST) {
		n += printf("\"const\"");
		isFirst = 0;
	}

	if (*qualifiers & AST_TQ_VOLATILE) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"volatile\"");
		isFirst = 0;
	}

	n += printf("]");

	return n;
}

void initASTStorageClassSpec(ASTStorageClassSpec *specs) {
	*specs = AST_SC_NONE;
}

int parseASTStorageClassSpec(ASTStorageClassSpec *specs, const Token *tok) {
	if (astErrMsg) {
		return 0;
	}

	switch (tok->type) {
		case TT_TYPEDEF:
			*specs |= AST_SC_TYPEDEF;
			return 1;
		case TT_AUTO:
			*specs |= AST_SC_AUTO;
			return 1;
		case TT_REGISTER:
			*specs |= AST_SC_REGISTER;
			return 1;
		case TT_STATIC:
			*specs |= AST_SC_STATIC;
			return 1;
		case TT_EXTERN:
			*specs |= AST_SC_EXTERN;
			return 1;
		default:
			return 0;
	}
}

int printASTStorageClassSpec(const ASTStorageClassSpec *specs) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*specs & AST_SC_TYPEDEF) {
		n += printf("\"typedef\"");
		isFirst = 0;
	}

	if (*specs & AST_SC_AUTO) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"auto\"");
		isFirst = 0;
	}

	if (*specs & AST_SC_REGISTER) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"register\"");
	}

	if (*specs & AST_SC_STATIC) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"static\"");
	}

	if (*specs & AST_SC_EXTERN) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"extern\"");
	}

	n += printf("]");

	return n;
}

void initASTArithType(ASTArithType *type) {
	*type = AST_AT_NONE;
}

char _isArith(TokenType tokType) {
	switch (tokType) {
		case TT_CHAR:
		case TT_INT:
		case TT_SIGNED:
		case TT_UNSIGNED:
		case TT_SHORT:
		case TT_LONG:
			return 1;
		default:
			return 0;
	}
}

int parseASTArithType(ASTArithType *type, const Token *tok) {
	if (astErrMsg) {
		return 0;
	}

	switch (tok->type) {
		case TT_CHAR:
			*type |= AST_AT_CHAR;
			return 1;
		case TT_INT:
			*type |= AST_AT_INT;
			return 1;
		case TT_SIGNED:
			*type |= AST_AT_SIGNED;
			return 1;
		case TT_UNSIGNED:
			*type |= AST_AT_UNSIGNED;
			return 1;
		case TT_SHORT:
			*type |= AST_AT_SHORT;
			return 1;
		case TT_LONG:
			*type |= AST_AT_LONG;
			return 1;
		default:
			return 0;
	}
}

int printASTArithType(const ASTArithType *type) {
	char isFirst = 1;
	int n = 0;
	n += printf("[");

	if (*type & AST_AT_CHAR) {
		n += printf("\"char\"");
		isFirst = 0;
	}

	if (*type & AST_AT_INT) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"int\"");
		isFirst = 0;
	}

	if (*type & AST_AT_SIGNED) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"signed\"");
		isFirst = 0;
	}

	if (*type & AST_AT_UNSIGNED) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"unsigned\"");
		isFirst = 0;
	}

	if (*type & AST_AT_SHORT) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"short\"");
		isFirst = 0;
	}

	if (*type & AST_AT_LONG) {
		if (!isFirst) {
			n += printf(", ");
		}
		n += printf("\"long\"");
		isFirst = 0;
	}

	n += printf("]");

	return n;
}

int _parseASTIdentifier(char **identifier, const Token *tok) {
	if (astErrMsg) {
		return 0;
	}

	if (tok->type != TT_IDENTIFIER) {
		return 0;
	}
	*identifier = strdup(tok->contents);
	return 1;
}

void initASTTypeSpec(ASTTypeSpec *typeSpec) {
	initASTTypeQualifier(&typeSpec->qualifiers);
	initASTStorageClassSpec(&typeSpec->storage);
	typeSpec->typeSpecType = AST_TST_UNKNOWN;
}

void freeASTTypeSpec(ASTTypeSpec *typeSpec) {
	switch (typeSpec->typeSpecType) {
		case AST_TST_TYPEDEF:
			free(typeSpec->c.typedefName);
			break;
		default:
			break;
	}
}

int parseASTTypeSpec(ASTTypeSpec *typeSpec, const Token *tok) {
	int n = 0, res;
	if (astErrMsg) {
		return 0;
	}

	initASTTypeSpec(typeSpec);

	while (1) {
		if (tok[n].type == TT_VOID) {
			typeSpec->typeSpecType = AST_TST_VOID;
			n++;
		} else if (_isArith(tok[n].type)) {
			if (typeSpec->typeSpecType == AST_TST_UNKNOWN) {
				initASTArithType(&typeSpec->c.arith);
				n += parseASTArithType(&typeSpec->c.arith, tok + n);
				typeSpec->typeSpecType = AST_TST_ARITH;
			} else if (typeSpec->typeSpecType == AST_TST_ARITH) {
				n +=  parseASTArithType(&typeSpec->c.arith, tok + n);
			} else {
				astErrMsg = "Multiple types in type specifier";
				return 0;
			}
		} else if (tok[n].type == TT_IDENTIFIER) {
			//TODO: need to have dictionary of typedefs
			//typeSpec->typeSpecType = AST_TST_TYPEDEF;
			//n += _parseASTIdentifier(&typeSpec->c.typedefName, tok + n);
			break;
		} else if (tok[n].type == TT_STRUCT) {
			//TODO
		} else if (tok[n].type == TT_UNION) {
			//TODO
		} else if (tok[n].type == TT_ENUM) {
			//TODO
		} else if ((res = parseASTStorageClassSpec(&typeSpec->storage, tok + n))) {
			n += res;
		} else if ((res = parseASTTypeQualifier(&typeSpec->qualifiers, tok + n))) {
			n += res;
		} else {
			break;
		}
	}

	return n;
}

int printASTTypeSpec(const ASTTypeSpec *typeSpec) {
	int n = 0;

	n += printf("{");

	n += printf("\"Type Qualifiers\": ");
	n += printASTTypeQualifier(&typeSpec->qualifiers);

	n += printf(", \"Storage Class Specifiers\": ");
	n += printASTStorageClassSpec(&typeSpec->storage);

	printf(", \"type\": ");
	switch (typeSpec->typeSpecType) {
		case AST_TST_VOID:
			n += printf("\"void\"");
			break;
		case AST_TST_ARITH:
			n += printASTArithType(&typeSpec->c.arith);
			break;
		case AST_TST_TYPEDEF:
			n += printf("\"%s\"", typeSpec->c.typedefName);
			break;
		default:
			n += printf("\"unknown\"");
	}

	n += printf("}");

	return n;
}

void initASTDeclarator(ASTDeclarator *declarator) {
	declarator->type = AST_DT_UNKNOWN;
}

void freeASTDeclarator(ASTDeclarator *declarator) {
	switch (declarator->type) {
		case AST_DT_IDENTIFIER:
			free(declarator->c.identifier);
			break;
		default:
			break;
	}
}

//TODO: add other declarators 
int parseASTDeclarator(ASTDeclarator *declarator, const Token *tok) {
	int n = 0, res;

	initASTDeclarator(declarator);

	if (tok[n].type != TT_IDENTIFIER) {
		return 0;
	}


	declarator->type = AST_DT_IDENTIFIER;
	declarator->c.identifier = strdup(tok[n].contents);
	n++;

	return n;
}

int printASTDeclarator(const ASTDeclarator *declarator) {
	int n = 0;

	if (declarator->type == AST_DT_IDENTIFIER) {
		n += printf("\"%s\"", declarator->c.identifier);
	} else {
		n += printf("\"unknown\"");
	}

	return n;
}

void initASTDeclaration(ASTDeclaration *declaration) {
	initASTTypeSpec(&declaration->typeSpec);
	initDList(&declaration->declarators, sizeof(ASTDeclaration));
}

void freeASTDeclaration(ASTDeclaration *declaration) {
	freeASTTypeSpec(&declaration->typeSpec);
	freeDList(&declaration->declarators, (DListFreeFunc) freeASTDeclarator);
}

int parseASTDeclaration(ASTDeclaration *declaration, const Token *tok) {
	int n = 0, res;
	ASTDeclarator tempDeclarator;

	initASTDeclaration(declaration);

	if ((res = parseASTTypeSpec(&declaration->typeSpec, tok + n))) {
		n += res;
	} else {
		freeASTDeclaration(declaration);
		return 0;
	}

	//TODO: impliment multiple declarators
	if ((res = parseASTDeclarator(&tempDeclarator, tok + n))) {
		n += res;
		dlistApp(&declaration->declarators, &tempDeclarator);
	} else {
		freeASTDeclaration(declaration);
		return 0;
	}

	if (tok[n].type != TT_SEMI_COLON) {
		astErrMsg = "Expecteding ; at end of statement";
		freeASTDeclaration(declaration);
		return 0;
	}
	n++;

	return n;
}

int printASTDeclaration(const ASTDeclaration *declaration) {
	int n = 0;

	n += printf("{");

	n += printf("\"Type Specification\": ");
	n += printASTTypeSpec(&declaration->typeSpec);

	n += printf(", \"Declarators\": ");
	n += printDList(&declaration->declarators, (DListPrintFunc) printASTDeclarator);

	n += printf("}");

	return n;
}
