#! /bin/sh
if [ -f ".ProjectSetup" ]
then
    echo "-- First initialization"
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
    ln -s ../kiwi/cpp/ ./extern/.
    mv ./extern/cpp ./extern/kiwi
    if [ -d "./bin" ]
    then
        echo "-- bin directory already exists"
    else
        echo "-- Creating bin directory"
        mkdir ./bin
    fi
    if [ -L "./bin/shaders" ]
    then
        rm ./bin/shaders
    fi
    echo "-- Creating links for shaders"
    ln -s ./src/shaders/ bin/.
    
    touch .ProjectSetup
    echo "-- End of the setup script"
fi


