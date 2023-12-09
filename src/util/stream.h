#pragma once

#include <stdio.h>
typedef enum StreamType {
	STREAM_FILE,
	STREAM_STR,
} StreamType;

typedef struct Stream {
	StreamType type;
	union {
		FILE *file;
		const char *str;
	} content;
} Stream;

void initStreamFile(Stream *stream, FILE *file);
void initStreamStr(Stream *stream, const char *str);
int streamGetc(Stream *stream);
