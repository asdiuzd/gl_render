mkdir -p ../out/rain
../bin/render ../src/render 8.1.g_buffer \
~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
~/data/cambridge_seq_predict/ours/kingscollege_rain_pose \
../out/rain \
cambridge_intrinsics.txt
