FLAGS_WEB=-g -s WASM=1 -s USE_SDL=2
FLAGS_GCC=-g -lSDL2 -lGL -lGLEW -c

linux: src/main.cpp renderer opengldriver util glprogram glpipeline
	g++ -g -lSDL2 -lGL -lGLEW bin/linux/util.o bin/linux/renderer.o bin/linux/opengldriver.o bin/linux/glprogram.o bin/linux/glpipeline.o src/main.cpp

web: src/main.cpp renderer opengldriver util glprogram glpipeline
	cp src/index.html bin/index.html
	em++ bin/web/renderer.o bin/web/glprogram.o bin/web/opengldriver.o bin/web/util.o src/main.cpp $(FLAGS_WEB) -o bin/main.js --preload-file shaders/

dir:
	mkdir bin
	mkdir bin/web
	mkdir bin/linux

clean:
	rm -rf bin

util: src/util/util.h src/util/util.cpp
	g++ src/util/util.cpp $(FLAGS_GCC) -o bin/linux/util.o

renderer: src/renderer.h src/renderer.cpp
	g++ src/renderer.cpp $(FLAGS_GCC) -o bin/linux/renderer.o

opengldriver: src/opengldriver/opengldriver.h src/opengldriver/opengldriver.cpp
	g++ src/opengldriver/opengldriver.cpp $(FLAGS_GCC) -o bin/linux/opengldriver.o

glprogram: src/opengldriver/opengl/glprogram.cpp src/opengldriver/opengl/glprogram.h
	g++ src/opengldriver/opengl/glprogram.cpp $(FLAGS_GCC) -o bin/linux/glprogram.o

glpipeline: src/opengldriver/opengl3/glpipeline.cpp src/opengldriver/opengl3/glpipeline.h
	g++ src/opengldriver/opengl3/glpipeline.cpp $(FLAGS_GCC) -o bin/linux/glpipeline.o



