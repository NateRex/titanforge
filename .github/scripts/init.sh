#!/usr/bin/env bash

# Determine if we had a cache hit or miss
if [ -f "./vcpkg/vcpkg" ]; then
    echo "CACHE_HIT=true" >> $GITHUB_ENV
else
    echo "CACHE_HIT=false" >> $GITHUB_ENV
fi

# Run vcpkg initialization
cd vcpkg
./bootstrap-vcpkg.sh

# Install dependencies
./vcpkg install catch2:x64-linux