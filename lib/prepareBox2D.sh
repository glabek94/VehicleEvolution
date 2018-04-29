git clone https://github.com/erincatto/Box2D.git
cd Box2D/
git checkout 05ee3c3c22af9ac1e5d88061d0b473f814c8210f^ \
 Box2D/Box2D/Box2DConfig.cmake.in \
 Box2D/Box2D/CMakeLists.txt \
 Box2D/Box2D/UseBox2D.cmake \
 Box2D/CMakeLists.txt \
 Box2D/HelloWorld/CMakeLists.txt \
 Box2D/Testbed/CMakeLists.txt \
 Box2D/glew/CMakeLists.txt \
 Box2D/glfw/CMakeLists.txt
sed -i -e 's/cmake_minimum_required(VERSION 2.6)/cmake_minimum_required(VERSION 3.1)/g' Box2D/CMakeLists.txt 
sed -i '/project(Box2D)/ a set(CMAKE_CXX_STANDARD 11)' Box2D/CMakeLists.txt
mkdir build && cd build
cmake -D BOX2D_BUILD_EXAMPLES=OFF ../Box2D/
make Box2D
cd ../..
cp Box2D/build/Box2D/libBox2D.a lib/
rm -rf Box2D
