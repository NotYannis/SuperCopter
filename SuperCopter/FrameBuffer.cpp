#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	dwBufferSize = { (SHORT)SCREEN_WIDTH, (SHORT)SCREEN_HEIGHT };
	dwBufferCoord = { 0, 0 };
	rcRegion = { 0, 0, (SHORT)SCREEN_WIDTH - 1, (SHORT)SCREEN_HEIGHT - 1 };
}

/*
====================
clear

Fill the buffer with spaces to erase the previous frame
====================
*/
void FrameBuffer::clear() {
	for (int x = 0; x < SCREEN_WIDTH; ++x) {
		for (int y = 0; y < SCREEN_HEIGHT; ++y) {
			buffer[y][x].Char.AsciiChar = ' ';
			buffer[y][x].Attributes = 0;
		}
	}
}

CHAR_INFO (&FrameBuffer::getBuffer())[SCREEN_HEIGHT][SCREEN_WIDTH]{
	return buffer;
}