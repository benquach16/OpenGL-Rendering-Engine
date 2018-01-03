#include <stdio.h>
#include "renderer/renderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int argc, char** argv) {
	printf("hello, world!\n");
	Renderer app;
	app.run();

	return 0;
}
