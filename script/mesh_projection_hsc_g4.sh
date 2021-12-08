mkdir -p ../out/g4_hsc
../bin/render ../src/render 8.1.g_buffer \
~/Dataset/CambridgeLandmarks/GreatCourt/greatcourt.obj \
~/data/cambridge_seq_predict/hsc/greatcourt-seq4 \
../out/g4_hsc \
cambridge_intrinsics.txt
