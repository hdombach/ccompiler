#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "../token.h"

struct Token;

void logTokErr(const struct Token *tok, const char *fmt, ...);
/**
 * @brief internal errors which mean I did something wrong
 */
void logIntHead(const char *fmt, ...);
void logIntError(const char *fmt, ...);
void logTokIntError(const struct Token *tok, const char *fmt, ...);

int isLogDebug();
void logDebugHead(const char *fmt, ...);
void logTokDebug(const struct Token *tok, const char *fmt, ...);

int logAssert(int exp, char *file, int line, char *expStr);

#define LOG_ASSERT(exp) \
	logAssert(exp, __FILE__, __LINE__, #exp)

#define LOG_DEBUG_HEAD \
	logDebugHead("%s:%d", __FILE__, __LINE__);

#define LOG_DEBUG(msg) \
	logDebugHead("%s:%d", __FILE__, __LINE__); \
	printf("%s\n", msg);

#define TODO(msg) \
{ \
	logIntHead("%s:%d", __FILE__, __LINE__); \
	printf("%s\n", msg); \
}
