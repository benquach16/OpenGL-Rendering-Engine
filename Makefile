

default:
	cp src/index.html bin/index.html
	em++ src/main.cpp -s WASM=1 -o bin/main.js
