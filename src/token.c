#include <stdlib.h>
#include <stdio.h>

#include "token.h"

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

void printToken(Token *token) {
	printf("{type: \"%s\"", tokTypeStr(token->type));
	if (token->contents) {
		printf(", contents: \"%s\"", token->contents);
	}

	printf(", position, \"");
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
