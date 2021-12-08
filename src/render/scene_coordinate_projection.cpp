
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

struct PoseData
{
    std::string filename;
    Eigen::Quaternionf q;
    Eigen::Vector3f p;
    float focal;
};

inline std::vector<PoseData> load_cambridge_pose_txt(const std::string &filename, std::map<std::string, float> &focal_map)
{
    std::vector<PoseData> pose_data;
    cout << "----------" << endl;
    cout << filename << endl;
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
        // char header_line[2048];
        // // skip three useless lines
        // char *unused = fgets(header_line, 100, file);
        // unused = fgets(header_line, 100, file);
        // unused = fgets(header_line, 100, file);
        // char filename_buffer[2048];
        PoseData pose;
        int cnt = 0;
        // while (!feof(file))
        // {
        //     if (fscanf(file, "%f %f %f %f %f %f %f", &pose.p.x(), &pose.p.y(), &pose.p.z(), &pose.q.x(), &pose.q.y(), &pose.q.z(), &pose.q.w()) != 7)
        //     {
        //         break;
        //     }
        //     // convert to Twc
        //     pose.q.normalize();
        //     // pose.q = pose.q.conjugate();
        //     pose_data.push_back(pose);
        // }
        for (int i = 0; i < 1005; i++)
        {
            char filename[256];
            sprintf(filename, "/home/zhouhan/data/code/computer-vision/visual-localization/gl_render/sense_pose/frame-%06d.txt", i);
            cout << filename << endl;
            ifstream pose_file(filename);
            double rt[3][4];

            pose_file >> rt[0][0] >> rt[0][1] >> rt[0][2] >> rt[0][3] >> rt[1][0] >> rt[1][1] >> rt[1][2] >> rt[1][3] >> rt[2][0] >> rt[2][1] >> rt[2][2] >> rt[2][3];

            Eigen::Matrix3f rotation_matrix;
            rotation_matrix << rt[0][0], rt[0][1], rt[0][2],
                rt[1][0], rt[1][1], rt[1][2],
                rt[2][0], rt[2][1], rt[2][2];

            cout << rotation_matrix << endl;
            Eigen::Quaternionf rotation_q(rotation_matrix);
            PoseData pose;
            pose.q = rotation_q;
            pose.p.x() = rt[0][3];
            pose.p.y() = rt[1][3];
            pose.p.z() = rt[2][3];
            cout << rotation_matrix << endl;
            pose_data.push_back(pose);
        }
        fclose(file);
    }
    else
    {
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
    const std::string &intrinsic_fn)
{

    std::map<std::string, float> focal_map;
    auto poses_twc_train = load_cambridge_pose_txt(base_dir + "/dataset_train.txt", focal_map);
    std::cout << "-----" << base_dir + "/dataset_train.txt" << std::endl;

    std::vector<PoseData>
        poses_twc_all = poses_twc_train;

    cout << poses_twc_all.size() << endl;

    Eigen::Matrix3f intrinsics;
    ifstream intrinsic_if(intrinsic_fn);
    cout << intrinsic_fn << endl;
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            double v;
            intrinsic_if >> v;
            intrinsics(r, c) = v;
            cout << intrinsics(r, c) << " ";
        }
        cout << endl;
    }

    cout << "---------" << endl;
    cout << intrinsics << endl;

    const float norm_min = -100,
                norm_max = 100;

    Renderer renderer(shader_path.c_str(), shader_name.c_str(), obj_path.c_str());
    std::string filename;
    char buffer[256];
    int cnt = 0;
    for (auto &pose : poses_twc_all)
    {
        sprintf(buffer, "frame-%06d.txt", cnt);
        filename = buffer;
        std::string input_pose_fn = base_dir + filename;
        sprintf(buffer, "frame-%06d.png", cnt++);
        filename = buffer;
        std::string output_vis_fn = out_dir + filename;
        std::string output_bin_fn = out_dir + filename.replace(filename.end() - 3, filename.end(), "bin");
        cout << "input pose fn = " << input_pose_fn << endl;
        cout << "pose file name = " << pose.filename << endl;
        cout << "output bin fn = " << output_bin_fn << endl;
        cout << "output vis fn = " << output_vis_fn << endl;

        float *xyz_buffer = renderer.render_single_frame(pose.q, pose.p, intrinsics, input_pose_fn, out_dir, true);

        std::cout << "write binary file to: " << output_bin_fn << endl;
        ofstream ofbin(output_bin_fn.c_str(), ios::binary);
        ofbin.write((const char *)xyz_buffer, sizeof(float) * SCR_WIDTH * SCR_HEIGHT * 3);

        cv::Mat img(SCR_HEIGHT, SCR_WIDTH, CV_8UC3);
        unsigned char *img_data = (unsigned char *)img.data;
        float max_buffer = -1000, min_buffer = 1000;
        for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
        {
            max_buffer = max(max_buffer, xyz_buffer[i * 3 + 0]);
            max_buffer = max(max_buffer, xyz_buffer[i * 3 + 1]);
            max_buffer = max(max_buffer, xyz_buffer[i * 3 + 2]);

            min_buffer = min(min_buffer, xyz_buffer[i * 3 + 0]);
            min_buffer = min(min_buffer, xyz_buffer[i * 3 + 1]);
            min_buffer = min(min_buffer, xyz_buffer[i * 3 + 2]);

            img_data[i * 3 + 2] = (xyz_buffer[i * 3 + 0] - norm_min) / (norm_max - norm_min) * 255; // r
            img_data[i * 3 + 1] = (xyz_buffer[i * 3 + 1] - norm_min) / (norm_max - norm_min) * 255; // g
            img_data[i * 3 + 0] = (xyz_buffer[i * 3 + 2] - norm_min) / (norm_max - norm_min) * 255; // b
        }
        cout << "max, min: " << max_buffer << " " << min_buffer << endl;
        cv::flip(img, img, 0);
        cv::imwrite(output_vis_fn, img);

        delete xyz_buffer;
    }
}