# mkdir -p ../out/kingscollege/seq1
# mkdir -p ../out/kingscollege/seq2
# mkdir -p ../out/kingscollege/seq3
# mkdir -p ../out/kingscollege/seq4
# mkdir -p ../out/kingscollege/seq5
# mkdir -p ../out/kingscollege/seq6
# mkdir -p ../out/kingscollege/seq7
# mkdir -p ../out/kingscollege/seq8

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
# ~/Dataset/CambridgeLandmarks/KingsCollege/ \
# ../out/kingscollege/ \
# cambridge_intrinsics.txt

#mkdir -p ../out/OldHospital/seq1
#mkdir -p ../out/OldHospital/seq2
#mkdir -p ../out/OldHospital/seq3
#mkdir -p ../out/OldHospital/seq4
#mkdir -p ../out/OldHospital/seq5
#mkdir -p ../out/OldHospital/seq6
#mkdir -p ../out/OldHospital/seq7
#mkdir -p ../out/OldHospital/seq8
#mkdir -p ../out/OldHospital/seq9

#../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
#~/Dataset/CambridgeLandmarks/OldHospital/oldhospital_clean.obj \
#~/Dataset/CambridgeLandmarks/OldHospital/ \
#../out/OldHospital/ \
#cambridge_intrinsics.txt

 mkdir -p ../out/greatcourt/seq1
 mkdir -p ../out/greatcourt/seq2
 mkdir -p ../out/greatcourt/seq3
 mkdir -p ../out/greatcourt/seq4
 mkdir -p ../out/greatcourt/seq5

 ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
../data/cambridge_data/GreatCourt/greatcourt.obj \
../data/cambridge_data/GreatCourt \
 ../out/greatcourt/ \
 cambridge_intrinsics.txt
