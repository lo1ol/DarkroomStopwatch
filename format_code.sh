#!/usr/bin/env bash

clang-format-17 -i $(find src -name "*.cpp" -o -name "*.h")
