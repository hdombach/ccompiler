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

void tokenizerTest() {
	tokenizerTestIdentifier();
	tokenizerTestNum();
	tokenizerTestChar();
	tokenizerTestStr();
}
