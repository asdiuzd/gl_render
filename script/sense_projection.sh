# rm -rf ../out/sensetime_proj
# mkdir -p ../out/sensetime_proj
#  ../bin/render \
#  ../src/render \
# 8.1.g_buffer \
#  /home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/sensetime_crop_top_gray.obj \
#  ../sense_pose \
#  ../out/sensetime_proj \
# /home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/script/sense_intrinsics.txt

rm -rf ../out/sensetime_sc
mkdir -p ../out/sensetime_sc
rm -rf ../out/sensetime_seg
mkdir -p ../out/sensetime_seg
 ../bin/render \
 ../src/render \
8.1.g_buffer.scene_coordinate \
 /home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/sensetime_crop_top_gray.obj \
 ../sense_pose/ \
 ../out/sensetime_sc/ \
/home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/script/sense_intrinsics.txt
