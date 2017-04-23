

FLAGS=-g -s WASM=1 -s USE_SDL=2
all: src/main.cpp renderer opengldriver
	cp src/index.html bin/index.html
	em++ bin/renderer.o bin/opengldriver.o src/main.cpp $(FLAGS) -o bin/main.js

renderer: src/renderer.h src/renderer.cpp
	em++ src/renderer.cpp $(FLAGS) -o bin/renderer.o

opengldriver: src/opengldriver.h src/opengldriver.cpp
	em++ src/opengldriver.cpp $(FLAGS) -o bin/opengldriver.o

local: src/main.cpp
	g++ src/renderer.cpp src/opengldriver.cpp -lSDL2 src/main.cpp

