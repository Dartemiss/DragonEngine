#pragma once
#include <windows.h>
#include <stdio.h>


#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);


void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};
/*
static public final float map(float value,
	float start1, float stop1,
	float start2, float stop2) {
	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	String badness = null;
	if (outgoing != outgoing) {
		badness = "NaN (not a number)";

	}
	else if (outgoing == Float.NEGATIVE_INFINITY ||
		outgoing == Float.POSITIVE_INFINITY) {
		badness = "infinity";
	}
	if (badness != null) {
		final String msg =
			String.format("map(%s, %s, %s, %s, %s) called, which returns %s",
				nf(value), nf(start1), nf(stop1),
				nf(start2), nf(stop2), badness);
		PGraphics.showWarning(msg);
	}
	return outgoing;
}
*/


// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Dragon Engine"

