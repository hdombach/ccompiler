#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "../token.h"
#include "../argParser.h"
#include "../util/color.h"

void logTokErr(const struct Token *tok, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "[ERROR %s:%d,%d] ", tok->filename, tok->posLine, tok->posColumn);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void logTokIntError(const struct Token *tok, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	logIntHead(" %s:%d,%d", tok->filename, tok->posLine, tok->posColumn);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void logTokDebug(const struct Token *tok, const char *fmt, ...) {
	if (isLogDebug()) {
		va_list args;
		va_start(args, fmt);
		fprintf(stdout, "[DEBUG %s:%d,%d] ", tok->filename, tok->posLine, tok->posColumn);
		vfprintf(stdout, fmt, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
}

void logIntHead(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "[" COLOR_RED "INTERNAL ERROR");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, COLOR_RESET "] ");
	va_end(args);

}

void logIntError(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	logIntHead("");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}


int logAssert(int exp, char *file, int line, char *expStr) {
	if (!exp) {
		logIntError("%s:%d assert failed (%s)", file, line, expStr);
	}
	return exp;
}

void logDebugHead(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	printf("[DEBUG");
	vprintf(fmt, args);
	printf("]");
	va_end(args);
}

int isLogDebug() {
	return g_args.verbose;
}
