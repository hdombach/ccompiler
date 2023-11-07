#include <stdarg.h>
#include <stdio.h>

#include "log.h"
#include "../token.h"

void logTokErr(const struct Token *tok, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "[ERROR %s:%d,%d] ", tok->filename, tok->posLine, tok->posColumn);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void logTokDebug(const struct Token *tok, const char *fmt, ...) {
#ifdef DEBUG
	va_list args;
	va_start(args, fmt);
	fprintf(stdout, "[DEBUG %s:%d,%d] ", tok->filename, tok->posLine, tok->posColumn);
	vfprintf(stdout, fmt, args);
	fprintf(stdout, "\n");
	va_end(args);
#endif
}

void logIntError(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "[INTERNAL ERROR] ");
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