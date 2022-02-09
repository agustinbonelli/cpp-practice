#!/bin/bash
set -euxo pipefail

git_root=$(git rev-parse --show-toplevel)

build_directory_path=${git_root}/build

mkdir -p $build_directory_path

conan install -if $build_directory_path -pr=${git_root}/conan.profile conanfile.txt

# cmake -B $build_directory_path

# cmake --build $build_directory_path