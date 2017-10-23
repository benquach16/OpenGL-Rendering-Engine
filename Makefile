FLAGS_WEB=-g -s WASM=1 -s USE_SDL=2
FLAGS_GCC=-g -lSDL2 -lGL -lGLEW -c

all: dir src/main.cpp renderer opengldriver util glprogram
	cp src/index.html bin/index.html
	em++ bin/web/renderer.o bin/web/glprogram.o bin/web/opengldriver.o bin/web/util.o src/main.cpp $(FLAGS_WEB) -o bin/main.js --preload-file shaders/

dir:
	mkdir bin
	mkdir bin/web
	mkdir bin/linux

clean:
	rm -rf bin

util: src/util/util.h src/util/util.cpp
	em++ src/util/util.cpp $(FLAGS_WEB) -o bin/web/util.o
	g++ src/util/util.cpp $(FLAGS_GCC) -o bin/linux/util.o

renderer: src/renderer.h src/renderer.cpp
	em++ src/renderer.cpp $(FLAGS_WEB) -o bin/web/renderer.o
	g++ src/renderer.cpp $(FLAGS_GCC) -o bin/linux/renderer.o

opengldriver: src/opengldriver/opengldriver.h src/opengldriver/opengldriver.cpp
	em++ src/opengldriver/opengldriver.cpp $(FLAGS_WEB) -o bin/web/opengldriver.o
	g++ src/opengldriver/opengldriver.cpp $(FLAGS_GCC) -o bin/linux/opengldriver.o

glprogram: src/opengldriver/glprogram.cpp src/opengldriver/glprogram.h
	em++ src/opengldriver/glprogram.cpp $(FLAGS_WEB) -o bin/web/glprogram.o
	g++ src/opengldriver/glprogram.cpp $(FLAGS_GCC) -o bin/linux/glprogram.o

glpipeline: src/opengldriver/glpipeline.cpp src/opengldriver/glpipeline.h
	em++ src/opengldriver/glpipeline.cpp $(FLAGS_WEB) -o bin/web/glpipeline.o
	g++ src/opengldriver/glpipeline.cpp $(FLAGS_GCC) -o bin/linux/glpipeline.o

local: src/main.cpp renderer opengldriver util
	g++ -lSDL2 -lGL -lGLEW src/util/util.cpp src/renderer.cpp src/opengldriver/opengldriver.cpp src/opengldriver/glprogram.cpp src/main.cpp

