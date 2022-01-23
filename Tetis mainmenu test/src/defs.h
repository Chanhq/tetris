/*
Copyright (C) 2021 Parallel Realities. All rights reserved.
*/

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define STRCPY(dest, src) strncpy(dest, src, sizeof(dest)); dest[sizeof(dest) - 1] = '\0'
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'

#define SCREEN_WIDTH           1280
#define SCREEN_HEIGHT          720

#define FPS                    60.0
#define LOGIC_RATE             (FPS / 1000)

#define MAX_FILENAME_LENGTH    256
#define MAX_NAME_LENGTH        32

#define MAX_KEYBOARD_KEYS      350

enum {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};
