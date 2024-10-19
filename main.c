#include <stdio.h>
#include "mercury.c"

void fun(void) {
	printf("Hello World\n");
}

int main(int argc, char** argv) {
	Mercury_Init();

	Mercury_Window* window = Mercury_CreateWindow("Test de fenetre", 800, 600);

	Mercury_ButtonFeature button;
	button.x = 50;
	button.y = 50;
	button.w = 50;
	button.h = 30;
	button.function = *fun;

	Mercury_Feature feature;
	feature.button = button;
	feature.type = MERCURY_BUTTON;

	Mercury_AddFeature(window, &feature);

	while (Mercury_HandleEvents(window)) {
		Mercury_UpdateFeatures(window);
		Mercury_RenderWindow(window);
	}

	Mercury_DestroyWindow(window);

	Mercury_Quit();
	return 0;
}
