#ifndef __FRAMEBUFFER__
#define __FRAMEBUFFER__

#include <Windows.h>
#include "Global.cpp"

class FrameBuffer {
	public :
		HANDLE hOutput;

		COORD dwBufferSize;
		COORD dwBufferCoord;
		SMALL_RECT rcRegion;



		FrameBuffer();
		void clear();
		CHAR_INFO (&getBuffer())[SCREEN_HEIGHT][SCREEN_WIDTH];

	private :
		CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

};

#endif
