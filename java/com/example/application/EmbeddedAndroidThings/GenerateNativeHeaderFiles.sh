#!/bin/sh

cd $(dirname "$0")

javac -h . *.java

for file in *.java
do
    rm ${file%.*}.class
    mv *${file%.*}.h ../../../../../cpp/EmbeddedAndroidThings/Include/${file%.*}.h
done
