git clone https://github.com/SFML/SFML.git
cd SFML/
mkdir build && cd build
cmake -D SFML_BUILD_AUDIO=false ..
make sfml-graphics sfml-system sfml-window
cd ../..
cp SFML/build/lib/* lib/
rm -rf SFML
