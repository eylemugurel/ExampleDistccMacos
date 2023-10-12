#!/bin/bash

if [ "$(uname)" != "Darwin" ]; then
	echo "This script is intended to run on macOS only."
	exit 1
fi

readonly BUILD_DIR="_build/Xcode"

if [ -d "$BUILD_DIR" ]; then
	rm -rf "$BUILD_DIR"
fi

cmake -B "$BUILD_DIR" -G Xcode -DCMAKE_SUPPRESS_REGENERATION=ON

open "$BUILD_DIR/ExampleDistccMacos.xcodeproj"
