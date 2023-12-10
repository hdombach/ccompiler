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

void tokenizerTestMacroIfndef () {
	tStartSection("Tokenizer macroifndef");

	tTokensSuccess(
		"$ifndef\n",
		(TokenType[]) {TT_MACRO_IFNDEF, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#ifndef",
		(TokenType[]) {TT_MACRO_IFNDEF, TT_EOF});

	tTokensSuccess(
		"#ifndef DEBUG\n"
		"#define DEBUG\n"
		"#endif\n",
		(TokenType[]) {
			TT_MACRO_IFNDEF, TT_IDENTIFIER, TT_NEWLINE, TT_EOF,
			TT_MACRO_DEFINE, TT_IDENTIFIER, TT_NEWLINE, TT_EOF,
			TT_MACRO_ENDIF, TT_NEWLINE, TT_EOF,
		});
}

void tokenizerTestMacroDefine() {
	tStartSection("Tokenizer macro define");

	tTokensSuccess(
		"#define DEFINE\n", 
		(TokenType[]) {TT_MACRO_DEFINE, TT_IDENTIFIER, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#define LOG(VALUE) printf(VALUE)\n", 
		(TokenType[]) {
			TT_MACRO_DEFINE, TT_IDENTIFIER, TT_O_PARAN, TT_IDENTIFIER, TT_C_PARAN,
			TT_IDENTIFIER, TT_O_PARAN, TT_IDENTIFIER, TT_C_PARAN, TT_NEWLINE, TT_EOF,
		});

	tTokensFailed(
		"#definee\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroUndef() {
	tStartSection("Tokenizer macro undef");

	tTokensSuccess(
		"#undef DEBUG\n", 
		(TokenType[]) {TT_MACRO_UNDEF, TT_IDENTIFIER, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#ifdef TEST\n"
		"#undef TEST\n"
		"#endif\n",
		(TokenType[]) {
			TT_MACRO_IFDEF, TT_IDENTIFIER, TT_NEWLINE,
			TT_MACRO_UNDEF, TT_IDENTIFIER, TT_NEWLINE,
			TT_MACRO_ENDIF, TT_NEWLINE, TT_EOF,
		});

	tTokensFailed(
		"#undeff\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroInclude() {
	tStartSection("Tokenizer macro include");

	tTokensSuccess(
		"#include <stdio.h>\n",
		(TokenType[]) {
			TT_MACRO_INCLUDE, TT_LESS, TT_IDENTIFIER,
			TT_PERIOD, TT_IDENTIFIER, TT_GREATER, TT_NEWLINE, TT_EOF,
		});

	tTokensSuccess(
		"#include \"stdio.h\"\n",
		(TokenType[]) {
			TT_MACRO_INCLUDE, TT_STR_CONSTANT, TT_NEWLINE, TT_EOF,
		});

	tTokensSuccess(
		"#include\n",
		(TokenType[]) {TT_MACRO_INCLUDE, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#includee\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroLine() {
	tStartSection("Tokenizer macro line");

	tTokensSuccess(
		"#line 152\n",
		(TokenType[]) {TT_MACRO_LINE, TT_NUMB_CONSTANT, TT_NEWLINE, TT_EOF});

	tTokensSuccess(
		"#line 152",
		(TokenType[]) {TT_MACRO_LINE, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess(
		"#line 12 \"reee.cpp\"\n",
		(TokenType[]) {
			TT_MACRO_LINE, TT_NUMB_CONSTANT, TT_STR_CONSTANT, TT_NEWLINE, TT_EOF
		});

	tTokensFailed(
		"#linee 14\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroError() {
	tStartSection("Tokenizer macro error");

	tTokensSuccess(
		"#error \"oh nooo\"\n",
		(TokenType[]) {TT_MACRO_ERROR, TT_STR_CONSTANT, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#errorror", 
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestMacroPragma() {
	tStartSection("Tokenier macro pragma");

	tTokensSuccess(
		"#pragma once\n",
		(TokenType[]) {TT_MACRO_PRAGMA, TT_IDENTIFIER, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#pragmaa", 
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

void tokenizerTestOpenCurly() {
	tStartSection("Tokenizer open curly");

	tTokensSuccess(
		"{",
		(TokenType[]) {TT_O_CURLY, TT_EOF});

	tTokensSuccess(
		"{{,{",
		(TokenType[]) {TT_O_CURLY, TT_O_CURLY, TT_COMMA, TT_O_CURLY, TT_EOF});

	tTokensSuccess(
		"{\"{\"",
		(TokenType[]) {TT_O_CURLY, TT_STR_CONSTANT, TT_EOF});

	tTokensSuccess(
		"{\n"
		"{",
		(TokenType[]) {TT_O_CURLY, TT_O_CURLY, TT_EOF});
}

void tokenizerTestCloseCurly() {
	tStartSection("Tokenizer close curly");

	tTokensSuccess(
		"}",
		(TokenType[]) {TT_C_CURLY, TT_EOF});

	tTokensSuccess(
		"{}",
		(TokenType[]) {TT_O_CURLY, TT_C_CURLY, TT_EOF});

	tTokensSuccess(
		"{{value}}", 
		(TokenType[]) {TT_O_CURLY, TT_O_CURLY, TT_IDENTIFIER, TT_C_CURLY, TT_C_CURLY, TT_EOF});
}

void tokenizerTestBraces() {
	tStartSection("Tokenizer test braces");

	tTokensSuccess(
		"[", 
		(TokenType[]) {TT_O_BRACE, TT_EOF});

	tTokensSuccess(
		"]", 
		(TokenType[]) {TT_C_BRACE, TT_EOF});

	tTokensSuccess(
		"[[4],[hi]]",
		(TokenType[]) {
			TT_O_BRACE, TT_O_BRACE, TT_NUMB_CONSTANT, TT_C_BRACE, TT_COMMA,
			TT_O_BRACE, TT_IDENTIFIER, TT_C_BRACE, TT_C_BRACE, TT_EOF,
		});
}

void tokenizerTestParam() {
	tStartSection("tokenizer test param");

	tTokensSuccess(
		"(",
		(TokenType[]) {TT_O_PARAN, TT_EOF});

	tTokensSuccess(
		")",
		(TokenType[]) {TT_C_PARAN, TT_EOF});

	tTokensSuccess(
		"(-)++)",
		(TokenType[]) {TT_O_PARAN, TT_MINUS, TT_C_PARAN, TT_INC, TT_C_PARAN, TT_EOF});

	tTokensSuccess(
		"({])}",
		(TokenType[]) {TT_O_PARAN, TT_O_CURLY, TT_C_BRACE, TT_C_PARAN, TT_C_CURLY, TT_EOF});
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
	tokenizerTestMacroDefine();
	tokenizerTestMacroUndef();
	tokenizerTestMacroInclude();
	tokenizerTestMacroLine();
	tokenizerTestMacroError();
	tokenizerTestMacroPragma();
	tokenizerTestOpenCurly();
	tokenizerTestCloseCurly();
	tokenizerTestBraces();
	tokenizerTestParam();
}
