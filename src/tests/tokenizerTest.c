#include "tokenizerTest.h"
#include "test.h"
#include "tools.h"
#include "../tok/token.h"

static void tokenizerTestIdentifier() {
	tStartSection("Tokenizer identifier");
	tTokensSuccess(
			"hello world",
			(TokenType[]) {TT_IDENTIFIER, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess(
			"hello world aaaah",
			(TokenType[]) {TT_IDENTIFIER, TT_IDENTIFIER, TT_IDENTIFIER, TT_EOF});
}

static void tokenizerTestNum() {
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

	tTokensSuccess(
		"5.2f 42.E4 31.5L .123",
		(TokenType[]) {TT_NUMB_CONSTANT, TT_NUMB_CONSTANT, TT_NUMB_CONSTANT, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestChar() {
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

static void tokenizerTestStr() {
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

static void tokenizerTestMacroIf() {
	tStartSection("Tokenizer macro if");

	tTokensSuccess(
		"#if 1\n"
		"#if HELLO",
		(TokenType[]) {TT_MACRO_IF, TT_NUMB_CONSTANT, TT_NEWLINE, TT_MACRO_IF, TT_IDENTIFIER, TT_EOF});

	tTokensFailed(
			"#iff", 
			(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

static void tokenizerTestMacroElif() {
	tStartSection("Tokenizer macro elif");

	tTokensSuccess(
		"#elif\n"
		"#define DEBUG",
		(TokenType[]) {TT_MACRO_ELIF, TT_NEWLINE, TT_MACRO_DEFINE, TT_IDENTIFIER, TT_EOF});

	tTokensFailed(
			"#elseif\n",
			(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

static void tokenizerTestMacroElse() {
	tStartSection("Tokenizer macro else");

	tTokensSuccess(
			"#else\n", 
			(TokenType[]) {TT_MACRO_ELSE, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#elsee\n",
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

static void tokenizerTestMacroEndif() {
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

static void tokenizerTestMacroIfdef() {
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

static void tokenizerTestMacroIfndef () {
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

static void tokenizerTestMacroDefine() {
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

static void tokenizerTestMacroUndef() {
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

static void tokenizerTestMacroInclude() {
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

static void tokenizerTestMacroLine() {
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

static void tokenizerTestMacroError() {
	tStartSection("Tokenizer macro error");

	tTokensSuccess(
		"#error \"oh nooo\"\n",
		(TokenType[]) {TT_MACRO_ERROR, TT_STR_CONSTANT, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#errorror", 
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

static void tokenizerTestMacroPragma() {
	tStartSection("Tokenier macro pragma");

	tTokensSuccess(
		"#pragma once\n",
		(TokenType[]) {TT_MACRO_PRAGMA, TT_IDENTIFIER, TT_NEWLINE, TT_EOF});

	tTokensFailed(
		"#pragmaa", 
		(CError[]) {CERR_TOKENIZER, CERR_UNKNOWN});
}

static void tokenizerTestOpenCurly() {
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

static void tokenizerTestCloseCurly() {
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

static void tokenizerTestBraces() {
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

static void tokenizerTestParam() {
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

static void tokenizerTestSemicolon() {
	tStartSection("Tokenizer test semi colon");

	tTokensSuccess(
			";",
			(TokenType[]) {TT_SEMI_COLON, TT_EOF});

	tTokensSuccess(
			";;", 
			(TokenType[]) {TT_SEMI_COLON, TT_SEMI_COLON, TT_EOF});

	tTokensSuccess(
			"[;]+23;", 
			(TokenType[]) {TT_O_BRACE, TT_SEMI_COLON, TT_C_BRACE, TT_PLUS, TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF});
}

static void tokenizerTestColon() {
	tStartSection("Tokenizer test colon");

	tTokensSuccess(
			":", 
			(TokenType[]) {TT_COLON, TT_EOF});

	tTokensSuccess(
			"value ? 2 : 4", 
			(TokenType[]) {TT_IDENTIFIER, TT_QUESTION, TT_NUMB_CONSTANT, TT_COLON, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess("int test:4", (TokenType[]) {TT_INT, TT_IDENTIFIER, TT_COLON, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestDots() {
	tStartSection("Tokenizer test dots");

	tTokensSuccess("...", (TokenType[]) {TT_DOTS, TT_EOF});

	tTokensSuccess("....", (TokenType[]) {TT_DOTS, TT_PERIOD, TT_EOF});
}

static void tokenizerTestQuestion() {
	tStartSection("Tokenizer test question");

	tTokensSuccess("?", (TokenType[]) {TT_QUESTION, TT_EOF});

	tTokensSuccess("??", (TokenType[]) {TT_QUESTION, TT_QUESTION, TT_EOF});

	tTokensSuccess(
			"value ? 2 : 4", 
			(TokenType[]) {TT_IDENTIFIER, TT_QUESTION, TT_NUMB_CONSTANT, TT_COLON, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestPeriod() {
	tStartSection("Tokenizer test period");

	tTokensSuccess(".", (TokenType[]) {TT_PERIOD, TT_EOF});

	tTokensSuccess("5.2", (TokenType[]) {TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess("thing.member", (TokenType[]) {TT_IDENTIFIER, TT_PERIOD, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("thing\n.\nmember", (TokenType[]) {TT_IDENTIFIER, TT_PERIOD, TT_IDENTIFIER, TT_EOF});
}

static void tokenizerTestArrow() {
	tStartSection("Tokenizer test arrow");

	tTokensSuccess("->", (TokenType[]) {TT_ARROW, TT_EOF});

	tTokensSuccess("-->>", (TokenType[]) {TT_DEC, TT_DBLE_GREATER, TT_EOF});

	tTokensSuccess("--->>", (TokenType[]) {TT_DEC, TT_ARROW, TT_GREATER, TT_EOF});

	tTokensSuccess("thing.member->member2", (TokenType[]) {
			TT_IDENTIFIER, TT_PERIOD, TT_IDENTIFIER, TT_ARROW, TT_IDENTIFIER, TT_EOF
	});
}

static void tokenizerTestTilde() {
	tStartSection("Tokenier test tilde");

	tTokensSuccess("~", (TokenType[]) {TT_TILDE, TT_EOF});

	tTokensSuccess("~01101001b", (TokenType[]) {TT_TILDE, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess("++~423-2", (TokenType[]) {TT_INC, TT_TILDE, TT_NUMB_CONSTANT, TT_MINUS, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestExclaim() {
	tStartSection("Tokenizer test exclaim");

	tTokensSuccess("!", (TokenType[]) {TT_EXCLAIM, TT_EOF});

	tTokensSuccess("!!hello", (TokenType[]) {TT_EXCLAIM, TT_EXCLAIM, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("(!4&hello)", (TokenType[]) {TT_O_PARAN, TT_EXCLAIM, TT_NUMB_CONSTANT, TT_AMP, TT_IDENTIFIER, TT_C_PARAN, TT_EOF});
}

static void tokenizerTestPlus() {
	tStartSection("Tokenizer test plus");

	tTokensSuccess("+", (TokenType[]) {TT_PLUS, TT_EOF});

	tTokensSuccess("+++", (TokenType[]) {TT_INC, TT_PLUS, TT_EOF});

	tTokensSuccess("hello+4", (TokenType[]) {TT_IDENTIFIER, TT_PLUS, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestMinus() {
	tStartSection("Tokenizer test minus");

	tTokensSuccess("-", (TokenType[]) {TT_MINUS, TT_EOF});

	tTokensSuccess("---", (TokenType[]) {TT_DEC, TT_MINUS, TT_EOF});

	tTokensSuccess("hello-4", (TokenType[]) {TT_IDENTIFIER, TT_MINUS, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestMult() {
	tStartSection("Tokenizer test mult");

	tTokensSuccess("*", (TokenType[]) {TT_MULT, TT_EOF});

	tTokensSuccess("*hello", (TokenType[]) {TT_MULT, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("**hello", (TokenType[]) {TT_MULT, TT_MULT, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("5*2-3", (TokenType[]) {TT_NUMB_CONSTANT, TT_MULT, TT_NUMB_CONSTANT, TT_MINUS, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestDiv() {
	tStartSection("Tokenizer test divide");

	tTokensSuccess("/", (TokenType[]) {TT_DIV, TT_EOF});

	tTokensSuccess("/ /", (TokenType[]) {TT_DIV, TT_DIV, TT_EOF});

	tTokensSuccess("adf/agd", (TokenType[]) {TT_IDENTIFIER, TT_DIV, TT_IDENTIFIER, TT_EOF});
}

static void tokenizerTestPerc() {
	tStartSection("Tokenizer test perc");

	tTokensSuccess("%", (TokenType[]) {TT_PERC, TT_EOF});

	tTokensSuccess("++(%%>", (TokenType[]) {TT_INC, TT_O_PARAN, TT_PERC, TT_PERC, TT_GREATER, TT_EOF});
}

static void tokenizerTestCaret() {
	tStartSection("Tokenizer test caret");

	tTokensSuccess("^", (TokenType[]) {TT_CARET, TT_EOF});

	tTokensSuccess("^^", (TokenType[]) {TT_CARET, TT_CARET, TT_EOF});

	tTokensSuccess("5^12", (TokenType[]) {TT_NUMB_CONSTANT, TT_CARET, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess("+^{", (TokenType[]) {TT_PLUS, TT_CARET, TT_O_CURLY, TT_EOF});
}

static void tokenizerTestAmp() {
	tStartSection("Tokenizer test amp");

	tTokensSuccess("&", (TokenType[]) {TT_AMP, TT_EOF});

	tTokensSuccess("&&&", (TokenType[]) {TT_DBLE_AMP, TT_AMP, TT_EOF});

	tTokensSuccess("5&12", (TokenType[]) {TT_NUMB_CONSTANT, TT_AMP, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess("+&{", (TokenType[]) {TT_PLUS, TT_AMP, TT_O_CURLY, TT_EOF});
}

static void tokenizerTestBar() {
	tStartSection("Tokenizer test bar");

	tTokensSuccess("|", (TokenType[]) {TT_BAR, TT_EOF});

	tTokensSuccess("|||", (TokenType[]) {TT_DBLE_BAR, TT_BAR, TT_EOF});

	tTokensSuccess("132|value", (TokenType[]) {TT_NUMB_CONSTANT, TT_BAR, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("{|)", (TokenType[]) {TT_O_CURLY, TT_BAR, TT_C_PARAN, TT_EOF});
}

static void tokenizerTestEql() {
	tStartSection("Tokenizer test equal");

	tTokensSuccess("=", (TokenType[]) {TT_EQL, TT_EOF});

	tTokensSuccess("===", (TokenType[]) {TT_DBLE_EQL, TT_EQL, TT_EOF});

	tTokensSuccess("value = 5", (TokenType[]) {TT_IDENTIFIER, TT_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestPlusEql() {
	tStartSection("Tokenizer test plus equal");

	tTokensSuccess("+=", (TokenType[]) {TT_PLUS_EQL, TT_EOF});

	tTokensSuccess("++=", (TokenType[]) {TT_INC, TT_EQL, TT_EOF});
	
	tTokensSuccess("+++=", (TokenType[]) {TT_INC, TT_PLUS_EQL, TT_EOF});

	tTokensSuccess("value += 4", (TokenType[]) {TT_IDENTIFIER, TT_PLUS_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestMinusEql() {
	tStartSection("Tokenizer test plus equal");

	tTokensSuccess("-=", (TokenType[]) {TT_MINUS_EQL, TT_EOF});

	tTokensSuccess("--=", (TokenType[]) {TT_DEC, TT_EQL, TT_EOF});
	
	tTokensSuccess("---=", (TokenType[]) {TT_DEC, TT_MINUS_EQL, TT_EOF});

	tTokensSuccess("value -= 4", (TokenType[]) {TT_IDENTIFIER, TT_MINUS_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestMultEql() {
	tStartSection("Tokenizer test mult equal");

	tTokensSuccess("*=", (TokenType[]) {TT_MULT_EQL, TT_EOF});

	tTokensSuccess("**=", (TokenType[]) {TT_MULT, TT_MULT_EQL, TT_EOF});
	
	tTokensSuccess("***=", (TokenType[]) {TT_MULT, TT_MULT, TT_MULT_EQL, TT_EOF});

	tTokensSuccess("value *= 4", (TokenType[]) {TT_IDENTIFIER, TT_MULT_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestDivEql() {
	tStartSection("Tokenier test divide equal");

	tTokensSuccess("/=", (TokenType[]) {TT_DIV_EQL, TT_EOF});

	tTokensSuccess("/ /=", (TokenType[]) {TT_DIV, TT_DIV_EQL, TT_EOF});

	tTokensSuccess("/ / /=", (TokenType[]) {TT_DIV, TT_DIV, TT_DIV_EQL, TT_EOF});

	tTokensSuccess("///=", (TokenType[]) {TT_EOF}); /* is a comment */

	tTokensSuccess("value /= 4", (TokenType[]) {TT_IDENTIFIER, TT_DIV_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestPercEql() {
	tStartSection("Tokenizer test perc equal");

	tTokensSuccess("%=", (TokenType[]) {TT_PERC_EQL, TT_EOF});

	tTokensSuccess("%%=", (TokenType[]) {TT_PERC, TT_PERC_EQL, TT_EOF});

	tTokensSuccess("value%=4;", (TokenType[]) {TT_IDENTIFIER, TT_PERC_EQL, TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF});
}

static void tokenizerTestCaretEql() {
	tStartSection("Tokenizer test caret equal");

	tTokensSuccess("^=", (TokenType[]) {TT_CARET_EQL, TT_EOF});

	tTokensSuccess("^^=", (TokenType[]) {TT_CARET, TT_CARET_EQL, TT_EOF});

	tTokensSuccess("value ^= 5;", (TokenType[]) {TT_IDENTIFIER, TT_CARET_EQL, TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF});
}

static void tokenizerTestAmpEql() {
	tStartSection("Tokenizer test amp equal");

	tTokensSuccess("&= ", (TokenType[]) {TT_AMP_EQL, TT_EOF});

	tTokensSuccess("&&=", (TokenType[]) {TT_DBLE_AMP, TT_EQL, TT_EOF});

	tTokensSuccess("&&&=", (TokenType[]) {TT_DBLE_AMP, TT_AMP_EQL, TT_EOF});

	tTokensSuccess("value &= 1;", (TokenType[]) {TT_IDENTIFIER, TT_AMP_EQL, TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF});
}

static void tokenizerTestBarEql() {
	tStartSection("Tokenizer test bar equal");

	tTokensSuccess("|=", (TokenType[]) {TT_BAR_EQL, TT_EOF});

	tTokensSuccess("||=", (TokenType[]) {TT_DBLE_BAR, TT_EQL, TT_EOF});

	tTokensSuccess("|||=", (TokenType[]) {TT_DBLE_BAR, TT_BAR_EQL, TT_EOF});

	tTokensSuccess("value |= 0b100101", (TokenType[]) {TT_IDENTIFIER, TT_BAR_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestDbleEql() {
	tStartSection("Tokenizer test double equal");

	tTokensSuccess("==", (TokenType[]) {TT_DBLE_EQL, TT_EOF});

	tTokensSuccess("===", (TokenType[]) {TT_DBLE_EQL, TT_EQL, TT_EOF});

	tTokensSuccess("value == 573", (TokenType[]) {TT_IDENTIFIER, TT_DBLE_EQL, TT_NUMB_CONSTANT, TT_EOF});

	tTokensSuccess(
			"value1 = value == 583;",
			(TokenType[]) {TT_IDENTIFIER, TT_EQL, TT_IDENTIFIER, TT_DBLE_EQL, TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF});
}

static void tokenizerTestNotEql() {
	tStartSection("Tokenizer test not equal");

	tTokensSuccess("!=", (TokenType[]) {TT_NOT_EQL, TT_EOF});

	tTokensSuccess("!!==", (TokenType[]) {TT_EXCLAIM, TT_NOT_EQL, TT_EQL, TT_EOF});

	tTokensSuccess("!value!=523", (TokenType[]) {TT_EXCLAIM, TT_IDENTIFIER, TT_NOT_EQL, TT_NUMB_CONSTANT, TT_EOF});
}

static void tokenizerTestLess() {
	tStartSection("Tokenizer test less");

	tTokensSuccess("<", (TokenType[]) {TT_LESS, TT_EOF});

	tTokensSuccess("5<!value", (TokenType[]) {TT_NUMB_CONSTANT, TT_LESS, TT_EXCLAIM, TT_IDENTIFIER, TT_EOF});
}

static void tokenizerTestGreater() {
	tStartSection("Tokenizer test greater");

	tStartSection("Tokenizer test greater");

	tTokensSuccess(">", (TokenType[]) {TT_GREATER, TT_EOF});

	tTokensSuccess("-5>~value", (TokenType[]) {TT_MINUS, TT_NUMB_CONSTANT, TT_GREATER, TT_TILDE, TT_IDENTIFIER, TT_EOF});

	tTokensSuccess("=<>!", (TokenType[]) {TT_EQL, TT_LESS, TT_GREATER, TT_EXCLAIM, TT_EOF});
}

static void tokenizerTestLessEql() {
	tStartSection("Tokenizer test less equal");

	tTokensSuccess("<=", (TokenType[]) {TT_LESS_EQL, TT_EOF});

	tTokensSuccess(
			"53<=2",
			(TokenType[]) {
				TT_NUMB_CONSTANT, TT_LESS_EQL, TT_NUMB_CONSTANT, TT_EOF
			});

	tTokensSuccess(
			"label<=!-(2)",
			(TokenType[]) {
				TT_IDENTIFIER, TT_LESS_EQL, TT_EXCLAIM, TT_MINUS,
				TT_O_PARAN, TT_NUMB_CONSTANT, TT_C_PARAN, TT_EOF
			});
}

static void tokenizerTestGreatEql() {
	tStartSection("Tokenizer test greater equal");

	tTokensSuccess(">=", (TokenType[]) {TT_GREATER_EQL, TT_EOF});

	tTokensSuccess(
		"53>=2",
		(TokenType[]) {
			TT_NUMB_CONSTANT, TT_GREATER_EQL, TT_NUMB_CONSTANT, TT_EOF
		});

	tTokensSuccess(
		"label>=!-(2)",
		(TokenType[]) {
			TT_IDENTIFIER, TT_GREATER_EQL, TT_EXCLAIM, TT_MINUS,
			TT_O_PARAN, TT_NUMB_CONSTANT, TT_C_PARAN, TT_EOF
		});
}

static void tokenizerTestLogicalComp() {
	tStartSection("Tokenizer test logical comparisons");

	tTokensSuccess("&&", (TokenType[]) {TT_DBLE_AMP, TT_EOF});

	tTokensSuccess("||", (TokenType[]) {TT_DBLE_BAR, TT_EOF});

	tTokensSuccess("&&||", (TokenType[]) {TT_DBLE_AMP, TT_DBLE_BAR, TT_EOF});

	tTokensSuccess(
		"thing&&!thing2||false",
		(TokenType[]) {
			TT_IDENTIFIER, TT_DBLE_AMP, TT_EXCLAIM, TT_IDENTIFIER, TT_DBLE_BAR,
			TT_IDENTIFIER, TT_EOF
		});
}

static void tokenizerTestBitwise() {
	tStartSection("Tokenizer test bitwise comparisons");

	tTokensSuccess("<<", (TokenType[]) {TT_DBLE_LESS, TT_EOF});

	tTokensSuccess(">>", (TokenType[]) {TT_DBLE_GREATER, TT_EOF});

	tTokensSuccess("<<=", (TokenType[]) {TT_DBLE_LESS_EQL, TT_EOF});

	tTokensSuccess(">>=", (TokenType[]) {TT_DBLE_GREATER_EQL, TT_EOF});

	tTokensSuccess(
		"(hello>>4)<<~3",
		(TokenType[]) {
			TT_O_PARAN, TT_IDENTIFIER, TT_DBLE_GREATER, TT_NUMB_CONSTANT, TT_C_PARAN,
			TT_DBLE_LESS, TT_TILDE, TT_NUMB_CONSTANT, TT_EOF
		});

	tTokensSuccess(
		"thing<<=hello>>4;",
		(TokenType[]) {
			TT_IDENTIFIER, TT_DBLE_LESS_EQL, TT_IDENTIFIER, TT_DBLE_GREATER,
			TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF
		});

	tTokensSuccess(
		"thing>>=hello<<4;",
		(TokenType[]) {
			TT_IDENTIFIER, TT_DBLE_GREATER_EQL, TT_IDENTIFIER, TT_DBLE_LESS,
			TT_NUMB_CONSTANT, TT_SEMI_COLON, TT_EOF
		});
}

static void tokenizerTestInc() {
	tStartSection("Tokenizer test increment");

	tTokensSuccess(
		"++", 
		(TokenType[]) {
			TT_INC, TT_EOF,
		});

	tTokensSuccess(
		"++++",
		(TokenType[]) {
			TT_INC, TT_INC, TT_EOF,
		});

	tTokensSuccess(
		"+++",
		(TokenType[]) {
			TT_INC, TT_PLUS, TT_EOF,
		});

	tTokensSuccess(
		"value+++hello",
		(TokenType[]) {
			TT_IDENTIFIER, TT_INC, TT_PLUS, TT_IDENTIFIER, TT_EOF,
		});

	tTokensSuccess(
		"+++=",
		(TokenType[]) {
			TT_INC, TT_PLUS_EQL, TT_EOF,
		});

	tTokensSuccess(
		"++++=",
		(TokenType[]) {
			TT_INC, TT_INC, TT_EQL, TT_EOF,
		});

}

static void tokenizerTestDec() {
	tStartSection("Tokenizer test decrement");

	tTokensSuccess(
		"--", 
		(TokenType[]) {
			TT_DEC, TT_EOF,
		});

	tTokensSuccess(
		"----",
		(TokenType[]) {
			TT_DEC, TT_DEC, TT_EOF,
		});

	tTokensSuccess(
		"---",
		(TokenType[]) {
			TT_DEC, TT_MINUS, TT_EOF,
		});

	tTokensSuccess(
		"value---hello",
		(TokenType[]) {
			TT_IDENTIFIER, TT_DEC, TT_MINUS, TT_IDENTIFIER, TT_EOF,
		});

	tTokensSuccess(
		"---=",
		(TokenType[]) {
			TT_DEC, TT_MINUS_EQL, TT_EOF,
		});

	tTokensSuccess(
		"----=",
		(TokenType[]) {
			TT_DEC, TT_DEC, TT_EQL, TT_EOF,
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
	tokenizerTestSemicolon();
	tokenizerTestColon();
	tokenizerTestDots();
	tokenizerTestQuestion();
	tokenizerTestPeriod();
	tokenizerTestArrow();
	tokenizerTestTilde();
	tokenizerTestExclaim();
	tokenizerTestPlus();
	tokenizerTestMinus();
	tokenizerTestMult();
	tokenizerTestDiv();
	tokenizerTestPerc();
	tokenizerTestCaret();
	tokenizerTestAmp();
	tokenizerTestBar();
	tokenizerTestEql();
	tokenizerTestPlusEql();
	tokenizerTestMinusEql();
	tokenizerTestMultEql();
	tokenizerTestDivEql();
	tokenizerTestPercEql();
	tokenizerTestCaretEql();
	tokenizerTestAmpEql();
	tokenizerTestBarEql();
	tokenizerTestDbleEql();
	tokenizerTestNotEql();
	tokenizerTestLess();
	tokenizerTestGreater();
	tokenizerTestLessEql();
	tokenizerTestGreatEql();
	tokenizerTestLogicalComp();
	tokenizerTestBitwise();	
	tokenizerTestInc();
	tokenizerTestDec();
}
