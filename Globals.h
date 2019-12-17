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

static float mapValues(float value,
	float start1, float stop1,
	float start2, float stop2) {

	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));

	return outgoing;
}



// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Dragon Engine"

