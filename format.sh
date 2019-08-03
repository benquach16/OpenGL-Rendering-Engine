#!/bin/bash
find ./src/ -path ./src/3rdparty -prune -o -iname *.h -o -iname *.cpp | xargs clang-format -i -style=WebKit
