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
    return files;
}

int main(int argc, char ** argv) {

    // argv[1] shader path
    // argv[2] obj path
    // argv[3] pose path
    Renderer renderer(argv[1], argv[2]);

    printf("123\n");

    // auto files = read_directory(argv[3]);
    // vector<string> pose_files;
    // for (auto &file : files) {
    //     if (file.find("normpose") != string::npos) {
    //         pose_files.emplace_back(file);
    //     }
    //     // printf("%s\n", files[i].c_str());
    // }

    
    // std::sort(pose_files.begin(), pose_files.end());

    
    // renderer.render_single_frame(rotation, translation, string(argv[4]));

    // ifstream pose_file(argv[3]);

    // while (1) {
    //     std::string line;
    //     Eigen::Quaternionf rotation;
    //     Eigen::Vector3f translation;
    //     string name;
    //     double qt[4];

    //     getline(pose_file, line);
    //     std::istringstream iss(line);
    //     iss >> name;
    //     iss >> qt[0] >> qt[1] >> qt[2];
    //     translation << qt[0], qt[1], qt[2];
    //     iss >> qt[0] >> qt[1] >> qt[2] >> qt[3];
    //     rotation = Eigen::Quaternionf(qt[0], qt[1], qt[2], qt[3]);

    //     std::cout << name << std::endl;
    //     // printf("%s\n", pose_files[i].c_str());
    //     renderer.render_single_frame(rotation, translation, string(argv[4]));
    // }



    return 0;
}
