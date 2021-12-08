mkdir -p ../out/k2_hsc
../bin/render ../src/render 8.1.g_buffer \
~/Dataset/CambridgeLandmarks/KingsCollege/kingscollege.obj \
~/data/cambridge_seq_predict/hsc/kings-seq2 \
../out/k2_hsc \
cambridge_intrinsics.txt
