#!/usr/bin/env bash

set -e

if [ "$1" == "build" ]; then
	# Rebuild.
	rm -rf build/ bin/
	cmake . -Bbuild
fi

# Compile.
cmake --build build

if [ "$1" == "readme" ]; then
	# Run readme helper.
	./build/readme-helper/sdptransform_readme_helper

	exit 0
fi

# Run test.
./build/test/test_sdptransform
