#pragma once

#include "../tok/token.h"

/**
 * @file
 *
 * @brief Tools for logging
 */

typedef struct Token Token;

/**
 * @brief A [x macro](https://en.wikipedia.org/wiki/X_macro) for CError
 */
#define X_CERROR \
	X(CERR_UNKNOWN, "unknown") \
	X(CERR_TOKENIZER, "tokenizer error") \
	X(CERR_IDENTIFIER, "Unexpected identifier") \
	X(CERR_MISSING_SEMI, "Missing semi colon") \
	X(CERR_TOK, "Unknown token") \
	X(CERR_BRACE, "Expecting a brace") \
	X(CERR_STRUCT, "Invalid struct or union") \
	X(CERR_ENUM, "Invalid enum") \
	X(CERR_MACRO, "Invalid macro") \
	X(CERR_INV_EXP, "Invalid expression") \
	X(CERR_EXP_STM, "Execting statement") \

/**
 * @brief The possible errors that can be thrown by the compile stages
 *
 * The errors aren't completely necessary since a more descriptive will
 * be printed to the screen. However, they are helpful in tests to check
 * if the compiler is correctly responding to incorrect code.
 * 
 * If the proper error isn't added yet, just use CERR_UNKNOWN
 */
#define X(NAME, STR) NAME,
typedef enum CError {
	X_CERROR
} CError;
#undef X

/**
 * @brief Bitfield listing different types of logs
 */
typedef enum LogLevel {
	/** @brief Messages for debugging */
	LL_DEBUG = 0b1,
	/** @brief General events or stats */
	LL_INFO = 0b10,
	/** @brief Something might be wrong */
	LL_WARNING = 0b100,
	/** @brief An error happened inside on of the compiler systems */
	LL_INT_ERROR = 0b1000,
	/** @brief An error in one of the compile stages */
	LL_CERROR = 0b10000,
} LogLevel;

extern LogLevel gLogLevel;

/**
 * @brief Sets up and resets internal cerr structures
 */
void initCerr();
/**
 * @brief Frees internal cerr structures
 */
void freeCerr();
/**
 * @brief The number of compile errors thrown
 *
 * @note This number can be reset by calling initCerr()
 */
int cerrCount();
/**
 * @brief Returns buffer of all compile errors thrown
 *
 * @note This buffer can be reset by calling initCerr()
 */
CError const *getCerr();
/**
 * @brief Logs a compile error
 * @param[in] err 
 * @param[in,opt] tok Location of the error
 * @param[in] fmt Format string similar to printf
 */
void logCerr(CError err, const struct Token *tok, const char *fmt, ...);
/**
 * @brief Converts CError to string representation
 * @param[in] err CError to convert
 * @returns String representation
 */
const char *cerrStr(CError err);

/**
 * @brief Logs a debug message
 * @param[in] file
 * @param[in] line
 * @param[in] label
 * @param[in] fmt Format string similar to printf
 *
 * @note Suggested you use DEBUG_MSG as it will autofill several parameters for you
 */
void logDebug(const char *file, int line, const char *label, const char *fmt, ...);
/**
 * @brief Logs a debug message
 * @param[in] msg Formated message similar to printf
 */
#define DEBUG_MSG(msg, ...) \
	logDebug(__FILE__, __LINE__, "DEBUG", msg __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Logs a debug message with token location info
 * @param[in] file
 * @param[in] line
 * @param[in] tok
 * @param[in] fmt Format string similar to printf
 *
 * @note Suggested you use DEBUG_TOK as it will autofill several parameters for you
 */
void logDebugTok(const char *file, int line, const Token *tok, const char *fmt, ...);
/**
 * @brief Logs a debug message with token location info
 * @param[in[ tok
 * @param[in] msg Format string similar to printf
 */
#define DEBUG_TOK(tok, msg, ...) \
	logDebugTok(__FILE__, __LINE__, tok, msg __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Verifies that program is in a correct state
 * @param[in] exp Expression to verify
 * @param[in] file
 * @param[in] line
 * @param[in] expStr String representation of the expression
 *
 * @note Suggested you use ASSERT as it will autofill several parameters for you
 */
int logAssert(int exp, char *file, int line, char *expStr);
/**
 * @brief Verifies that the program is in a correct state
 * @param[in] exp Expression to verify
 */
#define ASSERT(exp) \
	logAssert(exp, __FILE__, __LINE__, #exp)

/**
 * @brief Reminder that I needs to finish something
 * @param[in] file
 * @param[in] line
 * @param[in] fmt Format string similar to printf
 *
 * @note Suggested you use TODO as will autofill several parameters for you
 */
void logTodo(const char *file, int line, const char *fmt, ...);
/**
 * @brief Reminder that I need to finish something
 * @param[in] msg Format message similar to printf
 */
#define TODO(msg, ...) \
	logTodo(__FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Add an info log 
 * @param[in] fmt Format string similar to printf
 */
void logInfo(const char *fmt, ...);

/**
 * @brief Prints a test failed message
 * @param[in, opt] file
 * @param[in, opt] line
 * @param[in] section
 * @param[in, opt] msg
 * @param[in, opt] exp
 *
 * Used by the tests internally
 */
void logTestFailed(const char *file, int line, const char *section, const char *msg, const char *exp);

/**
 * @brief Throw an internal error
 * @param[in, opt] file
 * @param[in, opt] line
 * @param[in] fmt, args similar to fprint
 *
 * @note Suggested you use INT_ERROR as it will autofill several paramters for you
 */
void logIntError(const char *file, int line, const char *fmt, ...);

/**
 * @brief Throw an internal error
 * @param[in] fmt Format string similar to printf
 */
#define INT_ERROR(msg, ...) \
	logIntError(__FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__)
