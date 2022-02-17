mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-01
mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-02
mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-03
mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-04
mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-05
mkdir -p ~/data/SceneCoordinates/7scenes/chess/seq-06

../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
~/data/7scenes_release/tsdf/chess.ply \
~/data/7scenes_release/chess/ \
~/data/SceneCoordinates/7scenes/chess/ \
7scenes_intrinsics.txt \
chess

# mkdir -p ~/data/SceneCoordinates/7scenes/fire/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/fire/seq-02
# mkdir -p ~/data/SceneCoordinates/7scenes/fire/seq-03
# mkdir -p ~/data/SceneCoordinates/7scenes/fire/seq-04

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/fire.ply \
# ~/data/7scenes_release/fire/ \
# ~/data/SceneCoordinates/7scenes/fire/ \
# 7scenes_intrinsics.txt \
# fire

# mkdir -p ~/data/SceneCoordinates/7scenes/heads/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/heads/seq-02

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/heads.ply \
# ~/data/7scenes_release/heads/ \
# ~/data/SceneCoordinates/7scenes/heads/ \
# 7scenes_intrinsics.txt \
# heads

# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-02
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-03
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-04
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-05
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-06
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-07
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-08
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-09
# mkdir -p ~/data/SceneCoordinates/7scenes/office/seq-10

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/office.ply \
# ~/data/7scenes_release/office/ \
# ~/data/SceneCoordinates/7scenes/office/ \
# 7scenes_intrinsics.txt \
# office

# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-02
# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-03
# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-06
# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-07
# mkdir -p ~/data/SceneCoordinates/7scenes/pumpkin/seq-08

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/pumpkin.ply \
# ~/data/7scenes_release/pumpkin/ \
# ~/data/SceneCoordinates/7scenes/pumpkin/ \
# 7scenes_intrinsics.txt \
# pumpkin

# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-02
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-03
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-04
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-05
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-06
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-07
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-08
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-11
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-12
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-13
# mkdir -p ~/data/SceneCoordinates/7scenes/redkitchen/seq-14

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/redkitchen.ply \
# ~/data/7scenes_release/redkitchen/ \
# ~/data/SceneCoordinates/7scenes/redkitchen/ \
# 7scenes_intrinsics.txt \
# redkitchen

# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-01
# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-02
# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-03
# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-04
# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-05
# mkdir -p ~/data/SceneCoordinates/7scenes/stairs/seq-06

# ../bin/render ../src/render 8.1.g_buffer.scene_coordinate \
# ~/data/7scenes_release/tsdf/stairs.ply \
# ~/data/7scenes_release/stairs/ \
# ~/data/SceneCoordinates/7scenes/stairs/ \
# 7scenes_intrinsics.txt \
# stairs


cp -r ~/data/segmentations/7scenes/* ~/data/7scenes_release