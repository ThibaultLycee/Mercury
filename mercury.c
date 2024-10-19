#ifndef INCLUDE_MERCURY
#define INCLUDE_MERCURY

#include <SDL2/SDL.h>
#include <stdlib.h>

#define INBOUND(a, b, c) (a <= c && c <= b)

typedef struct {
	int type;
	int x;
	int y;
	int w;
	int h;
	int hovered;
	char* text;
	void (*function)();
} Mercury_ButtonFeature;

typedef union {
	int type;
	Mercury_ButtonFeature button;
} Mercury_Feature;

#define MERCURY_BUTTON 1

typedef struct Mercury_Feature_LL {
	struct Mercury_Feature_LL* next;
	Mercury_Feature* feature;
} Mercury_Feature_LL;

typedef struct {
	SDL_Window* window;
	Mercury_Feature_LL* features;
} Mercury_Window;

int Mercury_Init() {
	return SDL_Init(SDL_INIT_VIDEO);
}

void Mercury_Quit() {
	SDL_Quit();
}

Mercury_Window* Mercury_CreateWindow(char* name, int width, int height) {
	Mercury_Window* window = malloc(sizeof(Mercury_Window));
	window->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window->window == NULL) {
		SDL_Quit();
		return window;
	}
	return window;
};

void Mercury_DestroyWindow(Mercury_Window* window) {
	free(window->features);
	SDL_DestroyWindow(window->window);
}

void Mercury_AddFeature(Mercury_Window* window, Mercury_Feature* feature) {
	Mercury_Feature_LL* feature_LL = malloc(sizeof(Mercury_Feature_LL));
	feature_LL->feature = feature;
	feature_LL->next = window->features;
	window->features = feature_LL;
}

void Mercury_UpdateButton(Mercury_ButtonFeature* button) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (INBOUND(button->x, button->x + button->w, mouseX) && INBOUND(button->y, button->y + button->h, mouseY)) {
		button->hovered = 1;
	} else {
		button->hovered = 0;
	}
}

void Mercury_UpdateFeatures(Mercury_Window* window) {
	Mercury_Feature_LL* feature_LL = window->features;
        while (feature_LL != NULL) {
		Mercury_Feature* feature = feature_LL->feature;
		switch (feature->type) {
			case MERCURY_BUTTON:
				Mercury_UpdateButton(&feature->button);
				break;
			default:
				break;
		}
		feature_LL = feature_LL->next;
	}
}

void Mercury_HandleMouseEvent(Mercury_Window* window, SDL_Event* event) {
	Mercury_Feature_LL* feature_LL = window->features;
	while (feature_LL != NULL) {
		Mercury_Feature* feature = feature_LL->feature;
		switch (feature->type) {
			case MERCURY_BUTTON:
				Mercury_ButtonFeature button = feature->button;
				if (INBOUND(button.x, button.x + button.w, event->button.x) && INBOUND(button.y, button.y + button.h, event->button.y)) {
					(*button.function)();
				}
				break;
			default:
				break;
		}
		feature_LL = feature_LL->next;
	}
}

int Mercury_HandleEvents(Mercury_Window* window) {
	SDL_Event e;
	int returnValue = 1;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
				returnValue = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				Mercury_HandleMouseEvent(window, &e);
				break;
			default:
				returnValue = 1;
				break;
		}
	}
	return returnValue;
}

void Mercury_RenderWindow(Mercury_Window* window) {
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window->window);

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	Mercury_Feature_LL* feature_LL = window->features;
	while (feature_LL != NULL) {
		Mercury_Feature* feature = feature_LL->feature;
		switch(feature->type) {
			case MERCURY_BUTTON:
				Mercury_ButtonFeature button = feature->button;
				SDL_Rect rect = {button.x, button.y, button.w, button.h};
				if (feature->button.hovered)
					SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xFF, 0, 0));
				else
					SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0, 0xFF, 0));
				break;
			default:
				break;
		}
		feature_LL = feature_LL->next;
	}

	SDL_UpdateWindowSurface(window->window);
}

#endif
