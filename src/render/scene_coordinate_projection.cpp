
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

extern const float norm_min, norm_max;

bool process_cambridge = false;

struct PoseData
{
    std::string filename;
    Eigen::Quaternionf q;
    Eigen::Vector3f p;
    float focal;
};

bool load_sequences(const std::string& fn, vector<string>& seqs) {
    ifstream in(fn);

    while (!in.eof()) {
        string seq;
        in >> seq;
        if (seq == "") {
            continue;
        }
        seqs.emplace_back(seq);
    }
}

inline std::vector<PoseData> load_7scenes_poses(const string base_path, const string scene, bool gettraining, bool gettest) {
    cout << "load 7scenes poses" << endl;
    // fs::path base_path_str(base_path);
    string trainingset("TrainSplit.txt"), testset("TestSplit.txt");
    int frame_number;
    if (scene.compare("stairs") == 0) {
        frame_number = 500;
    } else {
        frame_number = 1000;
    }

    string training_fn = base_path + '/' + trainingset;
    string test_fn = base_path + '/' + testset;
    vector<string> seqs;
    std::vector<PoseData> pose_data;

    if (gettraining) {
        load_sequences(training_fn.c_str(), seqs);
    }
    if (gettest) {
        load_sequences(test_fn.c_str(), seqs);
    }

    /*
        The matrix stored in file is camera-to-world.
        We need world-to-camera.
    */
    stringstream s;
    s.fill('0');
    for (auto& seq : seqs) {
        for (int idx = 0; idx < frame_number; idx++) {
            // fs::path seq_str(seq);
            PoseData pose;
            s.str("");
            s << "frame-" << setw(6) << idx << ".pose.txt";
            pose.filename = seq + "/" + s.str();
            // auto fn = (base_path_str / seq_str / s.str()).string();
            auto fn = base_path + "/" + seq + "/" + s.str();
            // cout << "filename = " << fn << endl;
            ifstream in(fn);

            Eigen::Matrix4f e;
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    in >> e(r, c);
                    // cout << e(r, c) << ' ';
                }
            }
            // pose.p = e.block<3, 1>(0, 3) - Eigen::Vector3f(0.0245, 0, 0);
            pose.p = e.block<3, 1>(0, 3);
            // pose.p = pose.p + Eigen::Vector3f(0.0245, 0, 0);
            // pose.p = pose.p + e.block<3, 3>(0, 0) * (e.block<3, 3>(0, 0) * Eigen::Vector3f(0.0245, 0, 0));
            // pose.p = pose.p + e.block<3, 3>(0, 0) * (e.block<3, 3>(0, 0) * Eigen::Vector3f(0.0245, 0, 0));
            pose.p = pose.p + Eigen::Vector3f(0.0245, 0, 0);
            Eigen::Quaternionf q(e.block<3, 3>(0, 0));
            pose.q = q;
            // e.block<3, 1>(0, 3) = e.block<3, 1>(0, 3) - e.block<3, 3>(0, 0).inverse() * Eigen::Vector3f(0.0245, 0, 0);
            // q.normalize();
            // pose.q = q.conjugate();
            // pose.p = (pose.q * e.block<3, 1>(0, 3));
            // cout << pose.p(0) << ' ' << pose.p(1) << pose.p(2) << ' ' << endl;
            // Eigen::Quaternionf q(e.block<3, 3>(0, 0));
            // q.normalize();
            // e.block(0, 0, 3, 3) = q.conjugate().toRotationMatrix();
            // e.block(0, 3, 3, 1) = - (q.conjugate() * e.block(0, 3, 3, 1));
            // e.block(0, 0, 3, 3) = e.block(0, 0, 3, 3).inverse();
            // e.block(0, 0, 3, 3) = e.block(0, 0, 3, 3).transpose();
            // e.block(0, 3, 3, 1) = - (e.block(0, 0, 3, 3) * e.block(0, 3, 3, 1));
            // pose.p = 
            // es.emplace_back(e);
            pose_data.push_back(pose);
        }
    }

    return pose_data;
}

inline std::vector<PoseData> load_cambridge_pose_txt(const std::string &filename, std::map<std::string, float> &focal_map)
{
    std::vector<PoseData> pose_data;
    cout << "----------" << endl;
    cout << filename << endl;
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
        char header_line[2048];
        // skip three useless lines
        char *unused = fgets(header_line, 100, file);
        unused = fgets(header_line, 100, file);
        unused = fgets(header_line, 100, file);
        char filename_buffer[2048];
        PoseData pose;
        int cnt = 0;
        while (!feof(file))
        {
            if (fscanf(file, "%s %f %f %f %f %f %f %f", filename_buffer, &pose.p.x(), &pose.p.y(), &pose.p.z(), &pose.q.w(), &pose.q.x(), &pose.q.y(), &pose.q.z()) != 8)
            {
                break;
            }
            // convert to Twc
            pose.q.normalize();
            pose.q = pose.q.conjugate();
            pose.filename = filename_buffer;
            pose.focal = focal_map[pose.filename];

            // pose.q = pose.q.conjugate();
            // pose.p = - pose.q.toRotationMatrix() * pose.p;
            cout << filename_buffer << " " << pose.p << endl;
            pose_data.push_back(pose);
        }
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
    const std::string &intrinsic_fn,
    const std::string &dataset,
    const std::string &scene
    )
{

    // std::map<std::string, float> focal_map;
    // auto poses_twc_train = load_cambridge_pose_txt(base_dir + "/dataset_train.txt", focal_map);
    // auto poses_twc_test = load_cambridge_pose_txt(base_dir + "/dataset_test.txt", focal_map);

    // std::vector<PoseData> poses_twc_all = poses_twc_train;
    // poses_twc_all.insert(poses_twc_all.end(), poses_twc_test.begin(), poses_twc_test.end());

    cout << "dataset: " << dataset << endl;
    process_cambridge = dataset.compare("cambridge") == 0;
    cout << "process cambridge dataset: " << process_cambridge << endl;

    std::vector<PoseData> poses_twc_all;
    if (process_cambridge) {
        std::map<std::string, float> focal_map;
        auto poses_twc_train = load_cambridge_pose_txt(base_dir + "/dataset_train.txt", focal_map);
        auto poses_twc_test = load_cambridge_pose_txt(base_dir + "/dataset_test.txt", focal_map);

        poses_twc_all = poses_twc_train;
        poses_twc_all.insert(poses_twc_all.end(), poses_twc_test.begin(), poses_twc_test.end());
    } else {
        poses_twc_all = load_7scenes_poses(base_dir, scene, true, true);
    }


    cout << "pose size:" << poses_twc_all.size() << endl;

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

    

    Renderer renderer(shader_path.c_str(), shader_name.c_str(), obj_path.c_str());
    std::string filename;
    char buffer[256];
    int cnt = 0;
    for (auto &pose : poses_twc_all)
    {
        std::string output_vis_fn = out_dir + pose.filename;
        std::string input_pose_fn = base_dir + pose.filename;
        std::string output_bin_fn = out_dir + pose.filename;

        if (process_cambridge) {
            input_pose_fn = input_pose_fn.replace(input_pose_fn.end() - 3, input_pose_fn.end(), "txt");
            output_bin_fn = output_bin_fn.replace(output_bin_fn.end() - 3, output_bin_fn.end(), "bin");
        }
        else{
            input_pose_fn = input_pose_fn.replace(input_pose_fn.end() - 8, input_pose_fn.end(), "txt");
            output_bin_fn = output_bin_fn.replace(output_bin_fn.end() - 8, output_bin_fn.end(), "bin");
            output_vis_fn.replace(output_vis_fn.end() - 8, output_vis_fn.end(), "sc.png");
        }

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
        cout << output_vis_fn << endl;
        cv::imwrite(output_vis_fn, img);

        delete xyz_buffer;
    }
}