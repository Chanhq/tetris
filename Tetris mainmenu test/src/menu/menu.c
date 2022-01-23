/*
Copyright (C) 2021 Parallel Realities. All rights reserved.
*/

#include "menu.h"

static void logic(void);
static void draw(void);
static void start(void);
static void Score(void);
static void credits(void);
static void quit(void);

static char *message;

void initMenu(void)
{
	Widget *w;
	int x;

	x = 500;

	w = createWidget("start");
	w->x = x;
	w->y = 200;
	STRCPY(w->label, "Start");
	w->action = start;

	app.activeWidget = w;

	w = createWidget("Score");
	w->x = x;
	w->y = 250;
	STRCPY(w->label, "Score");
	w->action = Score;

	w = createWidget("credits");
	w->x = x;
	w->y = 300;
	STRCPY(w->label, "Credits");
	w->action = credits;

	w = createWidget("exit");
	w->x = x;
	w->y = 350;
	STRCPY(w->label, "Exit");
	w->action = quit;

	message = "Select a widget!";

	app.delegate.logic = &logic;
	app.delegate.draw = &draw;
}

static void logic(void)
{
	doWidgets();
}

static void draw(void)
{
	drawWidgets();

	drawText(message, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 65, 128, 190, 255, TEXT_ALIGN_CENTER, 0);
}

static void start(void)
{
	message = "Starting a new game.";
}

static void Score(void)
{
	message = "Showing Highscores.";
}

static void credits(void)
{
	message = "Our names for credits sake";
}

static void quit(void)
{
	exit(0);
}
