#pragma once

#include "token.h"
#include "util/dstr.h"
#include "util/stream.h"
#include "util/tokList.h"

/**
 * @file
 *
 * @brief Tools for creating token list from source code
 */

/**
 * @brief What is being aggregated in _TokenzState
 */
typedef enum {
	TOKENZ_STATE_NONE,
	TOKENZ_STATE_NUMBER,
	TOKENZ_STATE_STRING,
	TOKENZ_STATE_CHAR,
	TOKENZ_STATE_SYMBOL,
	TOKENZ_STATE_IDENTIFIER,
	TOKENZ_STATE_SINGLE_COMM,
	TOKENZ_STATE_MULTI_COMM,
	TOKENZ_STATE_MACRO,
} TokenzStateType;

/**
 * @brief The current state of the tokenizer
 *
 * Contains info like the word currently being aggregated
 * from the file and the current position inside the file.
 */
typedef struct _TokenzState {
	/** @brief The column position for the start of the aggregated word */
	int startColumn;
	/** @brief The line position for the start of the aggregated word */
	int startLine;
	/** @brief The current column position */
	int curColumn;
	/** @brief The current line position */
	int curLine;
	/** @brief The character currently being parsed */
	char curChar;
	/** @brief Whether the previous character was a backslash */
	char wasBackslash;
	/** @brief The file currently being parsed */
	const char *filename;
	/** @brief The word being aggregated */
	DStr curWord;
	/** @brief Whether a macro is currently being parsed */
	int isMacro;
	/** @brief What is being aggregated */
	TokenzStateType type;
	/** @brief The list of tokens already parsed */
	TokList tokens;
	/** @brief The symbol last successfully parsed */
	TokenType lastSymbolType;
} TokenzState;

/**
 * @param[out] state
 * @param[in] filename
 */
void initTokenzState(TokenzState *state, const char *filename);
/**
 * @param[in] state
 */
void freeTokenzState(TokenzState *state);

/**
 * @brief Primary function for tokenizing source code
 * @param[in] stream The source code
 * @param[in] filename 
 * @returns The parsed list of tokens
 */
TokList tokenize(Stream *stream, const char *filename);

