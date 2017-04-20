

FLAGS=-g -s WASM=1
all: src/main.cpp renderer opengldriver
	cp src/index.html bin/index.html
	em++ bin/renderer.o bin/opengldriver.o src/main.cpp $(FLAGS) -o bin/main.js

renderer: src/renderer.h src/renderer.cpp
	em++ src/renderer.cpp $(FLAGS) -s USE_SDL=2 -o bin/renderer.o

opengldriver: src/opengldriver.h src/opengldriver.cpp
	em++ src/opengldriver.cpp $(FLAGS) -o bin/opengldriver.o

