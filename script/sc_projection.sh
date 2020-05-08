mkdir -p ../out/kingscollege/seq1
mkdir -p ../out/kingscollege/seq2
mkdir -p ../out/kingscollege/seq3
mkdir -p ../out/kingscollege/seq4
mkdir -p ../out/kingscollege/seq5
mkdir -p ../out/kingscollege/seq6
mkdir -p ../out/kingscollege/seq7
mkdir -p ../out/kingscollege/seq8

../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
~/Dataset/CambridgeLandmarks/KingsCollege/ \
../out/kingscollege/ \
cambridge_intrinsics.txt
