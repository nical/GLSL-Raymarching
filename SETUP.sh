
if [ -f ".ProjectSetup" ]
then
    cd ../kiwi
    git pull origin master
    cd ../GLSL-Raymarcher
fi

touch .ProjectSetup

cd ..
git clone https://nical@github.com/nical/kiwi.git
#cd ./kiwi && git pull origin dev && cd ..
cd GLSL-Raymarcher

ln -s ../kiwi/cpp/ ./extern/kiwi

mkdir ./bin
ln -s ./src/shaders/ bin/.
