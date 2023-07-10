#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "tokenizer.h"
#include "util/dstr.h"

void initToken(Token *token) {
	token->type = TT_UNKNOWN;
	token->contents = NULL;
	token->posLine = 0;
	token->posColumn = 0;
	token->filename = NULL;
}

void freeToken(Token *token) {
	if (token->contents) {
		free(token->contents);
	}
	if (token->filename) {
		free(token->filename);
	}
}

void initNumbToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_INT_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
}

void initStrToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_STR_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);

}

void initCharToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_CHAR_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);

}

void initIdentToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_IDENTIFIER;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);

}

void initSymToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_UNKNOWN;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);

}

void printToken(Token *token) {
	printf("{\"type\": \"%s\"", tokTypeStr(token->type));
	if (token->contents) {
		printf(", \"contents\": \"");
		printJsonStr(token->contents);
		printf("\"");
	}

	printf(", \"position\": \"");
	if (token->filename) {
		printf("%s:", token->filename);
	}
	printf("%d:%d\"}", token->posLine, token->posColumn);
}

const char * TT_STRS[] = {
	"UNKNOWN",
	"Identifier",
	"Int Constant",
	"Char Constant",
	"String Constant",
	"If Macro",
	"Elif Macro",
	"Else Macro",
	"Endif Macro",
	"Ifdef Macro",
	"Ifndef Macro",
	//"Elifdef Macro",
	//"Elifndef Macro",
	"Define Macro",
	"Undef Macro",
	"Include Macro",
	//"Embed Macro",
	"Line Macro",
	"Error Macro",
	//"Warning Macro",
	"Pragma Macro",
	"Defined Macro",
	//"Has Include Macro",
	//"Has Embed Macro",
	//"Has C Attribute Macro",
	
	"{",
	"}",
	"[",
	"]",
	"(",
	")",
	";",
	":",
	"...",
	"?",
	//"::",
	".",
	"->",
	"~",
	"!",
	"+",
	"-",
	"*",
	"/",
	"%",
	"^",
	"&",
	"|",
	"=",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	"^=",
	"&=",
	"|=",
	"==",
	"!=",
	"<",
	">",
	"<=",
	">=",
	"&&",
	"||",
	"<<",
	">>",
	"<<=",
	">>=",
	"++",
	"--",
	",",

	//"alignas",
	//"alignof",
	"auto",
	//"bool",
	"break",
	"case",
	"char",
	"const",
	//"constexpr",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	//"false",
	"float",
	"for",
	"goto",
	"if",
	//"inline",
	"int",
	"long",
	//"nullptr",
	"register",
	//"restrict",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"static_assert",
	"struct",
	"switch",
	//"thread_local",
	//"true",
	"typedef",
	//"typeof",
	//"typeof_unequal",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while",
	//"_Alignas",
	//"_Alignof",
	//"_Atomic",
	//"_BitInt",
	//"_Bool",
	//"_Complex",
	//"_Decimal128",
	//"_Decimal32",
	//"_Decimal64",
	//"_Generic",
	//"_Imaginary",
	//"_Noreturn",
	//"_Static_assert",
	//"_Thread_local",
};
const char *tokTypeStr(TokenType type) {
	return TT_STRS[type];
}

TokenType findKeyword(const char *word) {
	for (TokenType type = TT_AUTO; type <= TT_WHILE; type++) {
		if (strcmp(tokTypeStr(type), word) == 0) {
			return type;
		}
	}
	return TT_UNKNOWN;
}

TokenType findPunctuation(const char *symb) {
	for (TokenType type = TT_O_CURLY; type <= TT_COMMA; type++) {
		if (strcmp(tokTypeStr(type), symb) == 0) {
			return type;
		}
	}
	return TT_UNKNOWN;
}
