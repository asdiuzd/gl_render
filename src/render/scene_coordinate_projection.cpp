
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <dirent.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <sys/types.h>
#include <dirent.h>

#include "scene_coordinate_projection.hpp"
 
struct PoseData {
    std::string filename;
    Eigen::Quaternionf q;
    Eigen::Vector3f p;
    float focal;
};

inline std::vector<PoseData> load_cambridge_pose_txt(const std::string &filename, std::map<std::string, float> &focal_map) {
    std::vector<PoseData> pose_data;
    if (FILE *file = fopen(filename.c_str(), "r")) {
        char header_line[2048];
        // skip three useless lines
        char *unused = fgets(header_line, 100, file);
        unused = fgets(header_line, 100, file);
        unused = fgets(header_line, 100, file);
        char filename_buffer[2048];
        PoseData pose;
        while (!feof(file)) {
            memset(filename_buffer, 0, 2048);
            if (fscanf(file, "%s %f %f %f %f %f %f %f", filename_buffer, &pose.p.x(), &pose.p.y(), &pose.p.z(), &pose.q.w(), &pose.q.x(), &pose.q.y(), &pose.q.z()) != 8) {
                break;
            }
            // convert to Twc
            pose.q.normalize();
            pose.q = pose.q.conjugate();
            pose.filename = filename_buffer;
            pose.focal = focal_map[pose.filename];
            pose_data.push_back(pose);
        }
        fclose(file);
    } else {
        std::cerr << "cannot open " << filename << "\n";
    }
    return pose_data;
}


void scene_coordinate_projection(
    const std::string &shader_path,
    const std::string &shader_name,
    const std::string &obj_path,
    const std::string &base_dir, 
    const std::string &out_dir,
    const std::string &intrinsic_fn
) {

    std::map<std::string, float> focal_map;
    auto poses_twc_train = load_cambridge_pose_txt(base_dir + "/dataset_train.txt", focal_map);
    auto poses_twc_test = load_cambridge_pose_txt(base_dir + "/dataset_test.txt", focal_map);

    std::vector<PoseData> poses_twc_all = poses_twc_train;
    poses_twc_all.insert(poses_twc_all.end(), poses_twc_test.begin(), poses_twc_test.end());

    Eigen::Matrix3f intrinsics;
    ifstream intrinsic_if(intrinsic_fn);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            intrinsic_if >> intrinsics(r, c);
        }
    }

    const float norm_min = -100, norm_max = 100;

    Renderer renderer(shader_path.c_str(), shader_name.c_str(), obj_path.c_str());
    for (auto& pose : poses_twc_all) {
        std::string input_pose_fn = base_dir + pose.filename;
        std::string output_vis_fn = out_dir + pose.filename;
        std::string output_bin_fn = out_dir + pose.filename.replace(pose.filename.end() - 3, pose.filename.end(), "bin");
        cout << "input pose fn = " << input_pose_fn << endl;
        cout << "pose file name = " << pose.filename << endl;
        cout << "output bin fn = " << output_bin_fn << endl;
        cout << "output vis fn = " << output_vis_fn << endl;

        float* xyz_buffer = renderer.render_single_frame(pose.q, pose.p, intrinsics, input_pose_fn, out_dir, true);

        std::cout << "write binary file to: " << output_bin_fn << endl;
        ofstream ofbin(output_bin_fn.c_str(), ios::binary);
        ofbin.write((const char *)xyz_buffer, sizeof(float) * SCR_WIDTH * SCR_HEIGHT * 3);

        cv::Mat img(SCR_HEIGHT, SCR_WIDTH, CV_8UC3);
        unsigned char *img_data = (unsigned char *)img.data;
        for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
        {
            img_data[i * 3 + 2] = (xyz_buffer[i * 3 + 0] - norm_min) / (norm_max - norm_min)* 255; // r
            img_data[i * 3 + 1] = (xyz_buffer[i * 3 + 1] - norm_min) / (norm_max - norm_min) * 255; // g
            img_data[i * 3 + 0] = (xyz_buffer[i * 3 + 2] - norm_min) / (norm_max - norm_min)* 255; // b
        }
        cv::flip(img, img, 0);
        cv::imwrite(output_vis_fn, img);

        delete xyz_buffer;
    }
}