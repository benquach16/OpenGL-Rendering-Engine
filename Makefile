

FLAGS=-g -s WASM=1 -s USE_SDL=2
all: src/main.cpp renderer opengldriver util glprogram
	cp src/index.html bin/index.html
	em++ bin/renderer.o bin/opengldriver.o bin/util.o src/main.cpp $(FLAGS) -o bin/main.js --preload-file shaders/

util: src/util.h src/util.cpp
	em++ src/util.cpp $(FLAGS) -o bin/util.o

renderer: src/renderer.h src/renderer.cpp
	em++ src/renderer.cpp $(FLAGS) -o bin/renderer.o

opengldriver: src/opengldriver.h src/opengldriver.cpp
	em++ src/opengldriver.cpp $(FLAGS) -o bin/opengldriver.o

glprogram: src/glprogram.cpp src/glprogram.h
	em++ src/glprogram.cpp $(FLAGS) -o bin/glprogram.o

local: src/main.cpp renderer opengldriver util
	g++ -lSDL2 -lGL -lGLEW src/util.cpp src/renderer.cpp src/opengldriver.cpp src/main.cpp

