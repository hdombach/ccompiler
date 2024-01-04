#pragma once

/**
 * @file
 * @brief Tools related to tokens
 */


/**
 * @brief A [x macro](https://en.wikipedia.org/wiki/X_macro) for CError
 */
#define X_TOKEN_TYPE \
	X(TT_UNKNOWN, "unknown token") \
	X(TT_IDENTIFIER, "identifier") \
	X(TT_NUMB_CONSTANT, "number constant") \
	X(TT_CHAR_CONSTANT, "char constant") \
	X(TT_STR_CONSTANT, "string constant") \
	X(TT_MACRO_IF, "#if") \
	X(TT_MACRO_ELIF, "#elif") \
	X(TT_MACRO_ELSE, "#else") \
	X(TT_MACRO_ENDIF, "#endif") \
	X(TT_MACRO_IFDEF, "#ifdef") \
	X(TT_MACRO_IFNDEF, "#ifndef") \
	U(TT_MACRO_ELIFDEF, "elifdef macro") \
	U(TT_MACRO_ELIFNDEF, "elifndef macro") \
	X(TT_MACRO_DEFINE, "#define") \
	X(TT_MACRO_UNDEF, "#undef") \
	X(TT_MACRO_INCLUDE, "#include") \
	U(TT_MACRO_EMBED, "embed macro") \
	X(TT_MACRO_LINE, "#line") \
	X(TT_MACRO_ERROR, "#error") \
	U(TT_MACRO_WARNING, "warning macro") \
	X(TT_MACRO_PRAGMA, "#pragma") \
	X(TT_MACRO_DEFINED, "#defined") \
	U(TT_MACRO_HAS_INCLUDE, "Has include macro") \
	U(TT_MACRO_HAS_EMBED, "has embed macro") \
	U(TT_MACRO_HAS_C_ATTRIBUTE, "has C attribute macro") \
	\
	X(TT_O_CURLY, "{") \
	X(TT_C_CURLY, "}") \
	X(TT_O_BRACE, "[") \
	X(TT_C_BRACE, "]") \
	X(TT_O_PARAN, "(") \
	X(TT_C_PARAN, ")") \
	X(TT_SEMI_COLON, ";") \
	X(TT_COLON, ":") \
	X(TT_DOTS, "...") \
	X(TT_QUESTION, "?") \
	U(TT_DBLE_COLON, "::") \
	X(TT_PERIOD, ".") \
	X(TT_ARROW, "->") \
	X(TT_TILDE, "~") \
	X(TT_EXCLAIM, "!") \
	X(TT_PLUS, "+") \
	X(TT_MINUS, "-") \
	X(TT_MULT, "*") \
	X(TT_DIV, "/") \
	X(TT_PERC, "%") \
	X(TT_CARET, "^") \
	X(TT_AMP, "&") \
	X(TT_BAR, "|") \
	X(TT_EQL, "=") \
	X(TT_PLUS_EQL, "+=") \
	X(TT_MINUS_EQL, "-=") \
	X(TT_MULT_EQL, "*=") \
	X(TT_DIV_EQL, "/=") \
	X(TT_PERC_EQL, "%=") \
	X(TT_CARET_EQL, "^=") \
	X(TT_AMP_EQL, "&=") \
	X(TT_BAR_EQL, "|=") \
	X(TT_DBLE_EQL, "==") \
	X(TT_NOT_EQL, "!=") \
	X(TT_LESS, "<") \
	X(TT_GREATER, ">") \
	X(TT_LESS_EQL, "<=") \
	X(TT_GREATER_EQL, ">=") \
	X(TT_DBLE_AMP, "&&") \
	X(TT_DBLE_BAR, "||") \
	X(TT_DBLE_LESS, "<<") \
	X(TT_DBLE_GREATER, ">>") \
	X(TT_DBLE_LESS_EQL, "<<=") \
	X(TT_DBLE_GREATER_EQL, ">>=") \
	X(TT_INC, "++") \
	X(TT_DEC, "--") \
	X(TT_COMMA, ",") \
	\
	U(TT_ALIGNAS, "alignas") \
	U(TT_ALIGNOF, "alignof") \
	X(TT_AUTO, "auto") \
	U(TT_BOOL, "bool") \
	X(TT_BREAK, "break") \
	X(TT_CASE, "case") \
	X(TT_CHAR, "char") \
	X(TT_CONST, "const") \
	U(TT_CONSTEXPR, "constexpr") \
	X(TT_CONTINUE, "continue") \
	X(TT_DEFAULT, "default") \
	X(TT_DO, "do") \
	X(TT_DOUBLE, "double") \
	X(TT_ELSE, "else") \
	X(TT_ENUM, "enum") \
	X(TT_EXTERN, "extern") \
	U(TT_FALSE, "false") \
	X(TT_FLOAT, "float") \
	X(TT_FOR, "for") \
	X(TT_GOTO, "goto") \
	X(TT_IF, "if") \
	U(TT_INLINE, "inline") \
	X(TT_INT, "int") \
	X(TT_LONG, "long") \
	U(TT_NULLPTR, "nullptr") \
	X(TT_REGISTER, "register") \
	U(TT_RESTRICT, "restrict") \
	X(TT_RETURN, "return") \
	X(TT_SHORT, "short") \
	X(TT_SIGNED, "signed") \
	X(TT_SIZEOF, "sizeof") \
	X(TT_STATIC, "static") \
	U(TT_STATIC_ASSERT, "static_assert") \
	X(TT_STRUCT, "struct") \
	X(TT_SWITCH, "switch") \
	U(TT_THREAD_LOCAL, "thread_local") \
	U(TT_TRUE, "true") \
	X(TT_TYPEDEF, "typedef") \
	U(TT_TYPEOF, "typeof") \
	U(TT_TYPEOF_UNEQUAL, "typeof_unequal") \
	X(TT_UNION, "union") \
	X(TT_UNSIGNED, "unsigned") \
	X(TT_VOID, "void") \
	X(TT_VOLATILE, "volatile") \
	X(TT_WHILE, "while") \
	U(TT__ALIGNAS, "_Alignas") \
	U(TT__ALIGNOF, "_Alignof") \
	U(TT__ATOMIC, "_Atomic") \
	U(TT__BIT_INT, "_BitInt") \
	U(TT__BOOL, "_Bool") \
	U(TT__COMPLEX, "_Complex") \
	U(TT__DECIMAL128, "_Decimal128") \
	U(TT__DECIMAL32, "_Decimal32") \
	U(TT__DECIMAL64, "_Decimal64") \
	U(TT__GENERIC, "_Generic") \
	U(TT__IMAGINARY, "_Imaginary") \
	U(TT__NORETURN, "_Noreturn") \
	U(TT__STATIC_ASSERT, "_Static_assert") \
	U(TT__THREAD_LOCAL, "_Thread_Local") \
	\
	X(TT_EOF, "EOF") \
	X(TT_NEWLINE, "\\n") \

#define U(NAME, STR)
#define X(NAME, STR) NAME,
/**
 * @brief All possible token types
 */
typedef enum TokenType {
	X_TOKEN_TYPE
} TokenType;

#undef X
#undef U

/**
 * @brief Discrete element parsed from source
 */
typedef struct Token {
	/** @brief Identifies type of token */
	TokenType type;
	/**
	 * @brief Copy of non-keyword token text
	 *
	 * @details Is a copy of the source code representating non-keywords
	 * like identifiers or constants
	 * - TT_IDENTIFIER: is the identifier name
	 * - TT_NUMB_CONSTANT: Is the ascii representation of the number (as seen in source code)
	 * - TT_STR_CONSTANT: Is the string itself (Without `"`)
	 * - TT_CHAR_CONSTANT: Is the character
	 *
	 *   @note Numberic constants encompess every type of number. (ints, floating points,
	 *   hex, binary). It is stored as a number and the actual value is parsed later
	 */
	char *contents;

	/** @brief Line position in source code */
	int posLine;
	/** @brief Column position in source code */
	int posColumn;
	/** @brief File that token came from */
	char *filename;
	/**
	 * @brief Whether token is part of token
	 *
	 * If a token is a macro, it does not necessarily mean it is a macro
	 * specific token. For example, replacement macros could contain any
	 * possible token
	 */
	int isMacro;
} Token;

/**
 * @param[out] token
 */
void initToken(Token *token);
/**
 * @param[in] token
 */
void freeToken(Token *token);

/** @brief The internal state of the tokenizer */
struct _TokenzState;

/**
 * @brief Creates number constant token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initNumbToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Creates a string constant token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initStrToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Creates a char constant token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initCharToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Creates an identifier or keyword token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initIdentToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Create non-alphab tokens
 * @param[out] token
 * @param[in] state Current state of tokenizer
 * @param[in] type Type of token to create
 *
 * Used for tokens like curly brackets which can't be aren't 
 * made with alpha-numberical characters
 */
void initSymToken(Token *token, const struct _TokenzState *state, TokenType type);
/**
 * @brief Creates a macro token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initMacroToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Creates an end of file token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 */
void initEOFToken(Token *token, const struct _TokenzState *state);
/**
 * @brief Creates a new line token
 * @param[out] token
 * @param[in] state Current state of tokenizer
 *
 * Newlines are used for identifiying the end of a macro.
 * If not part of a macro, newlines are ignored by the tokenizer
 */
void initNewlineToken(Token *token, const struct _TokenzState *state);

/**
 * @brief Copies a token into provided buffer
 * @param[out] dest Buffer to copy into
 * @param[in] tok Buffer to copy from
 */
void cpToken(Token *dest, Token const *tok);
/**
 * @brief Copies a token into newlly created buffer
 * @param[in] token Token to duplicate
 * @returns Newly created token buffer
 */
Token *dupToken(Token const *token);

/**
 * @brief Gets nest level for bracket tokens
 * @param[in] type
 * @returns numberic value representing change in bracket depth
 */
int tokenBracketDepth(TokenType type);

/**
 * @brief debug print the token
 * @param[in] token
 * @returns Number of characters printed
 */
int printToken(Token *token);

/**
 * @brief Prints the token as it appears in the file
 * @param[in] token
 * @returns Number of characters printed
 */
int printrToken(Token *token);
/**
 * @brief Static string representing token
 * @param[in] type
 * @returns static string
 */
const char *tokTypeStr(TokenType type);

/**
 * @brief Searches for if the word provided exactly matches a C keyword
 * @param[in] word
 * @returns The corresponding type if found, TT_UNKNOWN if not
 */
TokenType findKeyword(const char *word);

/**
 * @brief Searches for if the text provided match the start of a C symbol
 * @param[in] symb
 * @returns The corresponding type if found, TT_UNKNOWN if not
 */
TokenType findPunctuation(const char *symb);

/**
 * @brief Searches for if the word provided exactly matches a C macro
 * @param[in] word
 * @returns The corresponding type if found, TT_UNKNOWN if not
 */
TokenType findMacro(const char *word);
