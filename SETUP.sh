#! /bin/sh

if [ -d "../kiwi" ]
then
    cd ../kiwi/cpp
    echo "-- Pulling kiwi from git"
    git pull origin master
    echo "-- Building Kiwi"
    ./REBUILD.sh
    echo "-- Moving back to project directory"
    cd ../../GLSL-Raymarching
else
    cd ..
    echo "-- Cloning Kiwi from git"
    git clone https://nical@github.com/nical/kiwi.git
    cd ./kiwi/cpp
    echo "-- Building Kiwi"
    ./REBUILD.sh
    echo "-- Moving back to project directory"
    pwd
    cd ../../GLSL-Raymarching
    echo "-- Creating links for kiwi"
    ln -s ../../kiwi/cpp/ ./extern/
    mv ./extern/cpp ./extern/kiwi
    ls ./extern/kiwi/
fi

if [ -d "./bin" ]
then
    echo "-- bin directory already exists"
else
    echo "-- Creating bin directory"
    mkdir ./bin
fi

rm ./bin/shaders
echo "-- Creating links for shaders"
ln -s ../src/shaders/ ./bin/shaders
ls bin/shaders/

echo "-- Setup complete"

