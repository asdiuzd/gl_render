rm -rf ../mmw_out/sensetime_sc
mkdir -p ../mmw_out/sensetime_sc
rm -rf ../mmw_out/sensetime_seg
mkdir -p ../mmw_out/sensetime_seg
 ../bin/render \
 ../src/render \
8.1.g_buffer.scene_coordinate \
 /home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/mmw_sensetime.obj \
 ../mmw_pose/ \
 ../mmw_out/sensetime_sc/ \
/home/zhouhan/code/computer-vision/visual-localization/gl_render_backup/script/mmw_intrinsics.txt
