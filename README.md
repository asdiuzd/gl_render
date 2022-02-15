# gl_render

## Environment setup
```
sudo apt-get install cmake libeigen3-dev libglm-dev libglfw3 libglfw3-dev libassimp-dev libboost-all-dev libglew-dev libopencv-dev
mkdir build
cd build
cmake ..
make -j8
```
## Scene coordinate generation
```
cd script
./sc_projection.sh
```
The results are stored in the `out` directory. You need to set the path of the object file and the dataset. In the example script, we put the object file at `~/dataset/cambridge/CambridgeObj/KingsCollege/kingscollege.obj` and the dataset of the KingsCollege at `~/dataset/cambridge/KingsCollege`.