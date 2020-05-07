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
#include "util.hpp"
#include "gl_renderer.hpp"
#include <dirent.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <sys/types.h>
#include <dirent.h>
 
vector<string> read_directory(const std::string& name) {
    vector<string> files;
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        files.push_back(string(dp->d_name));
    }
    closedir(dirp);
    std::sort(files.begin(), files.end());
    return files;
}

int main(int argc, char ** argv) {

    // argv[1] shader path
    // argv[2] obj path
    // argv[3] pose path
    // argv[4] save_path
    // argv[5] start_idx
    Renderer renderer(argv[1], argv[2]);

    std::string folder(argv[3]);
    vector<string> filenames = read_directory(folder);
    // int start_idx = atoi(argv[5]);
    // printf("start idx %d\n", start_idx);
    int ct = 0;

    for (auto filename : filenames) {
        // if (++ct < start_idx) {
        //     continue;
        // }
        if (filename.find(".txt") == std::string::npos) continue;
        
        std::string fullname = folder + string("/") + filename;
        std::cout << fullname << std::endl;
        ifstream pose_file(fullname.c_str());
        double rt[3][4];

        pose_file >> rt[0][0] >> rt[0][1] >> rt[0][2] >> rt[0][3]
                    >> rt[1][0] >> rt[1][1] >> rt[1][2] >> rt[1][3]
                    >> rt[2][0] >> rt[2][1] >> rt[2][2] >> rt[2][3];
        
        Eigen::Matrix3f rotation_matrix;
        rotation_matrix << rt[0][0], rt[0][1], rt[0][2],
                            rt[1][0], rt[1][1], rt[1][2],
                            rt[2][0], rt[2][1], rt[2][2];
        Eigen::Quaternionf rotation_q(rotation_matrix);
        Eigen::Vector3f translation;
        translation << rt[0][3], rt[1][3], rt[2][3];

        Eigen::Matrix3f intrinsics;
        //  cambridge intrinsics
        intrinsics << 1673.274048, 0, 960,
                        0, 1673.274048, 540,
                        0, 0, 1;
        // seven scenes intrinsics
        // intrinsics << 384.35386606462447, 0, 319.28590839603237,
        //                 0, 384.9560729180638, 239.87334366520707,
        //                 0, 0, 1;

        // Eigen::Quaternionf tmp_q = rotation_q.conjugate();
        // rotation_q = tmp_q;
        // Eigen::Vector3f tmp_t = -(rotation_q * translation);
        // translation = tmp_t;

        std::cout << rotation_matrix << std::endl;
        std::cout << translation << std::endl;

        // renderer.render_single_frame(rotation_q, translation, intrinsics);
        renderer.render_single_frame(rotation_q, translation, intrinsics, filename, string(argv[4]));
    }


    return 0;
}
