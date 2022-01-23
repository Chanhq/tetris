/*
Copyright (C) 2021 Parallel Realities. All rights reserved.
*/

#include "widgets.h"

static Widget widgetHead;
static Widget *widgetTail;

void initWidgets(void)
{
	memset(&widgetHead, 0, sizeof(Widget));
	widgetTail = &widgetHead;
}

Widget *createWidget(char *name)
{
	Widget *w;

	w = malloc(sizeof(Widget));
	memset(w, 0, sizeof(Widget));
	widgetTail->next = w;
	w->prev = widgetTail;
	widgetTail = w;

	STRCPY(w->name, name);

	return w;
}

void doWidgets(void)
{
	if (app.keyboard[SDL_SCANCODE_UP])
	{
		app.keyboard[SDL_SCANCODE_UP] = 0;

		app.activeWidget = app.activeWidget->prev;

		if (app.activeWidget == &widgetHead)
		{
			app.activeWidget = widgetTail;
		}
	}

	if (app.keyboard[SDL_SCANCODE_DOWN])
	{
		app.keyboard[SDL_SCANCODE_DOWN] = 0;

		app.activeWidget = app.activeWidget->next;

		if (app.activeWidget == NULL)
		{
			app.activeWidget = widgetHead.next;
		}
	}

	if (app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (app.activeWidget->action != NULL)
		{
			app.activeWidget->action();
		}
	}
}

void drawWidgets(void)
{
	Widget *w;
	SDL_Color c;

	for (w = widgetHead.next ; w != NULL ; w = w->next)
	{
		if (w == app.activeWidget)
		{
			c.g = 255;
			c.r = c.b = 0;

			drawText(">", w->x - 40, w->y, c.r, c.g, c.b, TEXT_ALIGN_LEFT, 0);
		}
		else
		{
			c.r = c.g = c.b = 255;
		}

		drawText(w->label, w->x, w->y, c.r, c.g, c.b, TEXT_ALIGN_LEFT, 0);
	}
}
