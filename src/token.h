#pragma once

/*
 * All the token types
 */
typedef enum {
	TT_UNKNOWN = 0,
	TT_IDENTIFIER = 1,
	TT_INT_CONSTANT,
	TT_CHAR_CONSTANT,
	TT_STR_CONSTANT,
	TT_MACRO_IF,
	TT_MACRO_ELIF,
	TT_MACRO_ELSE,
	TT_MACRO_ENDIF,
	TT_MACRO_IFDEF,
	TT_MACRO_IFNDEF,
	//TT_MACRO_ELIFDEF,
	//TT_MACRO_ELIFNDEF,
	TT_MACRO_DEFINE,
	TT_MACRO_UNDEF,
	TT_MACRO_INCLUDE,
	//TT_MACRO_EMBED,
	TT_MACRO_LINE,
	TT_MACRO_ERROR,
	//TT_MACRO_WARNING,
	TT_MACRO_PRAGMA,
	TT_MACRO_DEFINED,
	//TT_MACRO_HAS_INCLUDE,
	//TT_MACRO_HAS_EMBED,
	//TT_MACRO_HAS_C_ATTRIBUTE,

	TT_O_CURLY,
	TT_C_CURLY,
	TT_O_BRACE,
	TT_C_BRACE,
	TT_O_PARAN,
	TT_C_PARAN,
	TT_SEMI_COLON,
	TT_COLON,
	TT_DOTS,
	TT_QUESTION,
	//TT_DBLE_COLON,
	TT_PERIOD,
	TT_ARROW,
	TT_TILDE,
	TT_EXCLAIM,
	TT_PLUS,
	TT_MINUS,
	TT_MULT,
	TT_DIV,
	TT_PERC,
	TT_CARET,
	TT_AMP,
	TT_BAR,
	TT_EQL,
	TT_PLUS_EQL,
	TT_MINUS_EQL,
	TT_MULT_EQL,
	TT_DIV_EQL,
	TT_PERC_EQL,
	TT_CARET_EQL,
	TT_AMP_EQL,
	TT_BAR_EQL,
	TT_DBLE_EQL,
	TT_NOT_EQL,
	TT_LESS,
	TT_GREATER,
	TT_LESS_EQL,
	TT_GREATER_EQL,
	TT_DBLE_AMP,
	TT_DBLE_BAR,
	TT_DBLE_LESS,
	TT_DBLE_GREATER,
	TT_DBLE_LESS_EQL,
	TT_DBLE_GREATER_EQL,
	TT_INC,
	TT_DEC,
	TT_COMMA,

	//TT_ALIGNAS,
	//TT_ALIGNOF,
	TT_AUTO,
	//TT_BOOL,
	TT_BREAK,
	TT_CASE,
	TT_CHAR,
	TT_CONST,
	//TT_CONSTEXPR,
	TT_CONTINUE,
	TT_DEFAULT,
	TT_DO,
	TT_DOUBLE,
	TT_ELSE,
	TT_ENUM,
	TT_EXTERN,
	//TT_FALSE,
	TT_FLOAT,
	TT_FOR,
	TT_GOTO,
	TT_IF,
	//TT_INLINE,
	TT_INT,
	TT_LONG,
	//TT_NULLPTR,
	TT_REGISTER,
	//TT_RESTRICT,
	TT_RETURN,
	TT_SHORT,
	TT_SIGNED,
	TT_SIZEOF,
	TT_STATIC,
	//TT_STATIC_ASSERT,
	TT_STRUCT,
	TT_SWITCH,
	//TT_THREAD_LOCAL,
	//TT_TRUE,
	TT_TYPEDEF,
	//TT_TYPEOF,
	//TT_TYPEOF_UNEQUAL,
	TT_UNION,
	TT_UNSIGNED,
	TT_VOID,
	TT_VOLATILE,
	TT_WHILE,
	//TT__ALIGNAS,
	//TT__ALIGNOF,
	//TT__ATOMIC,
	//TT__BIT_INT,
	//TT__BOOL,
	//TT__COMPLEX,
	//TT__DECIMAL128,
	//TT__DECIMAL32,
	//TT__DECIMAL64,
	//TT__GENERIC,
	//TT__IMAGINARY,
	//TT__NORETURN,
	//TT__STATIC_ASSERT,
	//TT__THREAD_LOCAL,


} TokenType;

typedef struct {
	TokenType type;
	char *contents;

	int posLine;
	int posColumn;
	char *filename;
	int isMacro;
} Token;

void initToken(Token *token);
void freeToken(Token *token);

struct _TokenzState;

void initNumbToken(Token *token, const struct _TokenzState *state);
void initStrToken(Token *token, const struct _TokenzState *state);
void initCharToken(Token *token, const struct _TokenzState *state);
void initIdentToken(Token *token, const struct _TokenzState *state);
void initSymToken(Token *token, const struct _TokenzState *state, TokenType type);
void initMacroToken(Token *token, const struct _TokenzState *state);

void tokenDup(Token const *token, Token *dest);
/*
 * Get a numeric value for if a token is a bracket and increases
 * or decreases depth
 */
int tokenBracketDepth(TokenType type);

void printToken(Token *token);
const char *tokTypeStr(TokenType type);

/*
 * Finds keyword token.
 * Has to match exactly.
 */
TokenType findKeyword(const char *word);
/*
 * Finds token type.
 * Characters have to match start of the token punctuation
 */
TokenType findPunctuation(const char *symb);

TokenType findMacro(const char *word);
