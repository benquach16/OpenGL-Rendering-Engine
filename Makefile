
all: src/main.cpp renderer
	cp src/index.html bin/index.html
	em++ bin/renderer.o src/main.cpp -s WASM=1 -o bin/main.js

renderer: src/renderer.h src/renderer.cpp
	em++ src/renderer.cpp -s WASM=1 -o bin/

