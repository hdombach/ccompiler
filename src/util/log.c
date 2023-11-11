#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "../token.h"
#include "../argParser.h"
#include "../util/color.h"

#ifdef DEBUG
const int isLogDebug = 1;
#else
const int isLogDebug = 0;
#endif

void logErrHead(const char *fmt, ...) {
	fprintf(stderr, "[" COLOR_RED);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, COLOR_RESET "] ");
}

void logErrTok(const struct Token *tok, const char *fmt, ...) {
	logErrHead("ERROR %s:%d,%d", tok->filename, tok->posLine, tok->posColumn);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

void logErr(const char *head, const char *fmt, ...) {
	logErrHead(head);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

void logDebugHead(const char *fmt, ...) {
	fprintf(stderr, "[");

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "] ");
}

void logDebugTok(const struct Token *tok, const char *fmt, ...) {
	logDebugHead("DEBUG %s:%d,%d", tok->filename, tok->posLine, tok->posColumn);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

void logDebug(const char *head, const char *fmt, ...) {
	logDebugHead(head);

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
}

int logAssert(int exp, char *file, int line, char *expStr) {
	if (!exp) {
		logErr("ASSERT FAILED %s:%d (%s)", file, line, expStr);
	}
	return exp;
}
