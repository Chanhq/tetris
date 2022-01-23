/*
Copyright (C) 2021 Parallel Realities. All rights reserved.
*/

typedef struct Texture Texture;
typedef struct Widget Widget;

struct Widget {
	char name[MAX_NAME_LENGTH];
	int x;
	int y;
	char label[MAX_NAME_LENGTH];
	Widget *prev;
	Widget *next;
	void (*action)(void);
};

struct Texture {
	char name[MAX_FILENAME_LENGTH];
	SDL_Texture *texture;
	Texture *next;
};

typedef struct {
	struct {
		void (*logic)(void);
		void (*draw)(void);
	} delegate;
	SDL_Renderer *renderer;
	SDL_Window *window;
	int keyboard[MAX_KEYBOARD_KEYS];
	double deltaTime;
	Widget *activeWidget;
	struct {
		int fps;
	} dev;
} App;
