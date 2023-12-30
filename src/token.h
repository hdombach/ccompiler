#pragma once

/**
 * @file
 * @brief Tools related to tokens
 */

/**
 * @brief All possible token types
 */
typedef enum {
	TT_UNKNOWN = 0,
	TT_IDENTIFIER = 1,
	TT_NUMB_CONSTANT = 2,
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

	TT_EOF,
	TT_NEWLINE, /* Is helpful for macros */
} TokenType;

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
