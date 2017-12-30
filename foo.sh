#!/usr/bin/env bash

set -e

if [ "$1" == "build" ]; then
	# Rebuild.
	rm -rf build/ bin/
	cmake . -Bbuild
fi

# Compile.
cmake --build build

# Run test.
./build/test/test_sdptransform
