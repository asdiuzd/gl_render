mkdir -p ../out/kings_seq2
../bin/render ../src/render 8.1.g_buffer \
~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
~/data/cambridge_seq_predict/ours/kingscollege_seq2_pose/kingscollege_seq2_pose \
../out/kings_seq2 \
cambridge_intrinsics.txt
