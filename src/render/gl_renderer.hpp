#ifndef GL_RENDERER_HPP
#define GL_RENDERER_HPP

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

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

extern const float norm_min, norm_max;

class Renderer {
public:
    Renderer(const std::string shader_path, const std::string shader_name, const std::string obj_path);
    ~Renderer();

    float* render_single_frame(Eigen::Quaternionf rotation, Eigen::Vector3f translation, Eigen::Matrix3f intrinsics,
                            string filename, string output_path = "", bool return_buffer = false);

private:

    Shader shaderGeometryPass;
    Shader shaderDebug;
    Model nanosuit;
    unsigned int gBuffer;
    unsigned int gAlbedoSpec;
    unsigned int rboDepth;

    void setup_opengl();


};

#endif