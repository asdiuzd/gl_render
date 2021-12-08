mkdir -p ../out/dusk_ours
../bin/render ../src/render 8.1.g_buffer \
~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
~/data/cambridge_seq_predict/ours/kingscollege_dusk_pose \
../out/dusk_ours \
cambridge_intrinsics.txt
