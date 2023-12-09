#include "tokenizerTest.h"
#include "test.h"
#include "tools.h"
#include "../token.h"

void tokenizerTestIdentifier() {
	tStartSection("Tokenizer identifier");
	tTokensSuccess(
			"hello world",
			(TokenType[]) {TT_IDENTIFIER, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess(
			"hello world aaaah",
			(TokenType[]) {TT_IDENTIFIER, TT_IDENTIFIER, TT_IDENTIFIER, TT_EOF});
}

void tokenizerTestNum() {
	tStartSection("Tokenizer numbers");

	tTokensSuccess(
		"143 hello 000",
		(TokenType[]) {TT_NUMB_CONSTANT, TT_IDENTIFIER, TT_NUMB_CONSTANT, TT_EOF});

	TODO("test things like 143Invalid");

	tTokensSuccess(
		"hello192 hel1o 123",
		(TokenType[]) {TT_IDENTIFIER, TT_IDENTIFIER, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess(
		"123,456+2",
		(TokenType[]) {TT_NUMB_CONSTANT, TT_COMMA, TT_NUMB_CONSTANT, TT_PLUS, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess(
		"{42{hi",
		(TokenType[]) {TT_O_CURLY, TT_NUMB_CONSTANT, TT_O_CURLY, TT_IDENTIFIER, TT_EOF});
}

void tokenizerTestChar() {
	tStartSection("Tokenizer char constants");

	tTokensSuccess(
		"'5'",
		(TokenType[]) {TT_CHAR_CONSTANT, TT_EOF});

	tTokensSuccess(
		"'\\\\''\\t'\n'e'",
		(TokenType[]) {TT_CHAR_CONSTANT, TT_CHAR_CONSTANT, TT_CHAR_CONSTANT, TT_EOF});

	tTokensSuccess(
			"'4'hello+'\'",
			(TokenType[]) {TT_CHAR_CONSTANT, TT_IDENTIFIER, TT_PLUS, TT_CHAR_CONSTANT, TT_EOF});
}

void tokenizerTestStr() {
	tStartSection("Tokenizer str constants");

	tTokensSuccess(
		"\"hello world\"",
		(TokenType[]) {TT_STR_CONSTANT, TT_EOF});

	tTokensSuccess(
		"\"hello \\\" world\\\"\"\"\"",
		(TokenType[]) {TT_STR_CONSTANT, TT_STR_CONSTANT, TT_EOF});

	tTokensSuccess(
		"\"\"+\" \"}{",
		(TokenType[]) {TT_STR_CONSTANT, TT_PLUS, TT_STR_CONSTANT, TT_C_CURLY, TT_O_CURLY, TT_EOF});

	tTokensSuccess(
		"123\"hello\"hello",
		(TokenType[]) {TT_NUMB_CONSTANT, TT_STR_CONSTANT, TT_IDENTIFIER, TT_EOF});
}

void tokenizerTestMacroIf() {
	tStartSection("Tokenizer macro if");

	tTokensSuccess(
		"#if 1\n"
		"#if HELLO",
		(TokenType[]) {TT_MACRO_IF, TT_NUMB_CONSTANT, TT_NEWLINE, TT_MACRO_IF, TT_IDENTIFIER, TT_EOF});

	tTokensFailed(
			"#iff", 
			(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroElif() {
	tStartSection("Tokenizer macro elif");

	tTokensSuccess(
		"#elif\n"
		"#define DEBUG",
		(TokenType[]) {TT_MACRO_ELIF, TT_NEWLINE, TT_MACRO_DEFINE, TT_IDENTIFIER, TT_EOF});

	tTokensFailed(
			"#elseif\n",
			(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroElse() {
	tStartSection("Tokenizer macro else");

	tTokensSuccess(
			"#else\n", 
			(TokenType[]) {TT_MACRO_ELSE, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#elsee\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroEndif() {
	tStartSection("Tokenizer macro endif");

	tTokensSuccess(
		"#endif\n", 
		(TokenType[]) {TT_MACRO_ENDIF, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#if 1 == thing()\n"
		"#else\n"
		"#endif",
		(TokenType[]) {
			TT_MACRO_IF, TT_NUMB_CONSTANT, TT_DBLE_EQL, TT_IDENTIFIER, TT_O_PARAN, TT_C_PARAN, TT_NEWLINE,
			TT_MACRO_ELSE, TT_NEWLINE,
			TT_MACRO_ENDIF, TT_EOF,
		});

	tTokensFailed(
		"#endiff", 
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroIfdef() {
	tStartSection("Tokenizer macro ifdef");

	tTokensSuccess(
		"#ifdef\n",
		(TokenType[]) {TT_MACRO_IFDEF, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#ifdef",
		(TokenType[]) {TT_MACRO_IFDEF, TT_EOF});

	tTokensFailed(
		"#ifdeff\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});

	tTokensSuccess(
		"#ifdef HELLO\n"
		"#define THING\n"
		"#endif\n",
		(TokenType[]) {
			TT_MACRO_IFDEF, TT_IDENTIFIER, TT_NEWLINE,
			TT_MACRO_DEFINE, TT_IDENTIFIER, TT_NEWLINE,
			TT_MACRO_ENDIF, TT_NEWLINE, TT_EOF,
		});
}

void tokenizerTest() {
	tokenizerTestIdentifier();
	tokenizerTestNum();
	tokenizerTestChar();
	tokenizerTestStr();
	tokenizerTestMacroIf();
	tokenizerTestMacroElif();
	tokenizerTestMacroEndif();
	tokenizerTestMacroIfdef();
}
