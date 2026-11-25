#!/bin/bash

set -e

clang-format -i -- include/cactus/*.hpp src/*.cpp

exit 0
