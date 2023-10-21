#!/bin/bash
set -euxo pipefail

CONAN_STORAGE=$(conan config get storage.path)

code -a  ${CONAN_STORAGE}