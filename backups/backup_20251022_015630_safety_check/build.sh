#!/usr/bin/env bash

export PATH="/mingw64/bin:$PATH"

echo "==============================="
echo "  Compiling MiniGit Project"
echo "==============================="

mkdir -p obj

# Compile only if source changed
for f in src/*.cpp; do
    obj_file="obj/$(basename "$f" .cpp).o"
    if [ ! -f "$obj_file" ] || [ "$f" -nt "$obj_file" ]; then
        echo "Compiling $f..."
        g++ -std=c++17 -Wall -Wextra -Iinclude -I. -c "$f" -o "$obj_file" || { echo "❌ Compilation failed for $f"; exit 1; }
    else
        echo "Skipping $f (up to date)"
    fi
done

# Detect subsystem flag fix
LINK_FLAGS="-static-libgcc -static-libstdc++"

echo "Linking..."
g++ obj/*.o -o mini_git.exe -mconsole $LINK_FLAGS -lssl -lcrypto -lstdc++fs
if [ $? -ne 0 ]; then
    echo "⚠️ Linking failed with -mconsole, trying -subsystem,console..."
    g++ obj/*.o -o mini_git.exe -Wl,-subsystem,console $LINK_FLAGS -lssl -lcrypto -lstdc++fs
    if [ $? -ne 0 ]; then
        echo "❌ Linking failed completely"
        exit 1
    fi
fi

echo "✅ Build successful!"
echo "Output: mini_git.exe"
