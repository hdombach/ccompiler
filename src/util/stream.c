#include "stream.h"
#include <stdio.h>

void initStreamFile(Stream *stream, FILE *file) {
	stream->type = STREAM_FILE;
	stream->content.file = file;
}

void initStreamStr(Stream *stream, const char *str) {
	stream->type = STREAM_STR;
	stream->content.str = str;
}

int streamGetc(Stream *stream) {
	if (stream->type == STREAM_FILE) {
		return getc(stream->content.file);
	} else {
		int result = *stream->content.str;
		if (result) stream->content.str++;
		return result;
	}
}
