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


GLFWwindow* window;

// settings
const unsigned int SCR_WIDTH = 1920/2;
const unsigned int SCR_HEIGHT = 1080/2;

const float near = 0.01;
const float far = 1000;

int return_value = 0;

#define DELETE_FRAME 1
#define NEXT_FRAME 2
#define LAST_FRAME 3
#define WITH_DRAW 4
#define SAVE_OTHER 5


// #define SHOW_DEPTH
// #define USE_CAM_IN_POSE




#ifndef USE_CAM_IN_POSE

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //     glfwSetWindowShouldClose(window, true);
    // }
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return_value = DELETE_FRAME;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return_value = LAST_FRAME;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return_value = NEXT_FRAME;
    }
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return_value = WITH_DRAW;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return_value = SAVE_OTHER;
    }
}
#else

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        printf("%f %f %f\n", camera.Position[0], camera.Position[1], camera.Position[2]);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, 0.05);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, 0.05);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, 0.05);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, 0.05);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        printf("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
            projection[0][0], projection[0][1], projection[0][2], projection[0][3],
            projection[1][0], projection[1][1], projection[1][2], projection[1][3],
            projection[2][0], projection[2][1], projection[2][2], projection[2][3],
            projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
        printf("-----------projection------------------\n");
        printf("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
            view[0][0], view[0][1], view[0][2], view[0][3],
            view[1][0], view[1][1], view[1][2], view[1][3],
            view[2][0], view[2][1], view[2][2], view[2][3],
            view[3][0], view[3][1], view[3][2], view[3][3]);
        printf("------------view-----------------\n");
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


#endif


class Renderer {
public:
    Renderer(const std::string shader_path, const std::string obj_path) {
        setup_opengl();
        shaderGeometryPass = Shader((shader_path + "/8.1.g_buffer.vs").c_str(),
                                (shader_path + "/8.1.g_buffer.fs").c_str());
        shaderDebug = Shader((shader_path + "/8.1.debug.vs").c_str(),
                            (shader_path + "/8.1.debug.fs").c_str());
        nanosuit = Model(obj_path);
        // printf("meshes size %lu\n", nanosuit.meshes.size());
        // printf("indices size %lu\n", nanosuit.meshes[0].indices.size());
        // printf("vertexs size %lu\n", nanosuit.meshes[0].vertices.size());
        // printf("textures size %lu\n", nanosuit.meshes[0].textures.size());
    }
    ~Renderer() {
        glDeleteFramebuffers(1, &gBuffer);  
        glfwTerminate();
    }

    int render_single_frame(Eigen::Quaternionf rotation, Eigen::Vector3f translation, Eigen::Matrix3f intrinsics,
                            string filename, string output_path = "") {

        // Twc to Tcw
        // Eigen::Matrix3f new_rotation_matrix = rotation_matrix.transpose().eval();
        // Eigen::Quaternionf rotation(new_rotation_matrix);
        // Eigen::Vector3f new_translation = - new_rotation_matrix * translation;

        float fov_y = std::atan(intrinsics(1, 2) / intrinsics(1, 1)) * 2;
        // fov_y = 100 * M_PI / 180;
        glm::mat4 projectionMatrix = glm::perspective(
            fov_y, // The vertical Field of View, in radians
            // width / height
            (float)(intrinsics(0, 2)*2) / (intrinsics(1, 2)*2),       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
            near,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
            far             // Far clipping plane. Keep as little as possible.
        );

        // glm::mat4 projection = convert2projection_matrix(intrinsics, SCR_WIDTH, SCR_HEIGHT);
        glm::mat4 projection = projectionMatrix;
        glm::mat4 view = convert2view_matrix(rotation, translation);

    #ifdef SHOW_DEPTH
        // render loop
        glfwSetWindowShouldClose(window, false);
        while (!glfwWindowShouldClose(window))
        {

    #endif

#ifdef USE_CAM_IN_POSE
        processInput(window);
#endif

        // render
        // ------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderGeometryPass.use();
        shaderGeometryPass.setMat4("projection", projection);
        shaderGeometryPass.setMat4("view", view);
        shaderGeometryPass.setMat4("model", glm::mat4(1.0f));
        nanosuit.Draw(shaderGeometryPass);

#ifndef SHOW_DEPTH
        // string filename = "/home/eugenelee/data/cambriage/"
        assert(output_path != "");
        string save_filename = filename;
        save_filename = save_filename.replace(save_filename.find("pose"), 4, "rgb");
        save_filename = save_filename.replace(save_filename.find("txt"), 3, "png");
        if (output_path != "") {
            save_filename = output_path + save_filename;
        }
        std::cout << save_filename << std::endl;
#endif


        static bool flag = true;
        if (flag) {
            flag = false;
            printf("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
                projection[0][0], projection[0][1], projection[0][2], projection[0][3],
                projection[1][0], projection[1][1], projection[1][2], projection[1][3],
                projection[2][0], projection[2][1], projection[2][2], projection[2][3],
                projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
            printf("-----------projection------------------\n");
            printf("%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n",
                view[0][0], view[0][1], view[0][2], view[0][3],
                view[1][0], view[1][1], view[1][2], view[1][3],
                view[2][0], view[2][1], view[2][2], view[2][3],
                view[3][0], view[3][1], view[3][2], view[3][3]);
            printf("------------view-----------------\n");
        }


        float *cam_xyz_buffer = new float[SCR_WIDTH*SCR_HEIGHT * 3];
        cv::Mat img(SCR_HEIGHT, SCR_WIDTH, CV_8UC3);
        unsigned char *img_data = (unsigned char *)img.data;
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_FLOAT, cam_xyz_buffer);
        for (int i = 0; i < SCR_WIDTH*SCR_HEIGHT; i++) {
            img_data[i * 3 + 2] = cam_xyz_buffer[i * 3 + 0] * 255; // r
            img_data[i * 3 + 1] = cam_xyz_buffer[i * 3 + 1] * 255; // g
            img_data[i * 3 + 0] = cam_xyz_buffer[i * 3 + 2] * 255; // b
        }
        cv::flip(img, img, 0);
        cv::imwrite(save_filename, img);
        // cv::imshow("test", img);
        // cv::waitKey(0);


        // glfwSetWindowShouldClose(window, true);
#ifndef SHOW_DEPTH
        // fclose(depth_file);
#endif

#ifdef USE_CAM_IN_POSE
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
#endif

    #ifdef SHOW_DEPTH
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderDebug.use();
            shaderDebug.setMat4("projection", projection);
            shaderDebug.setMat4("view", view);
            shaderDebug.setMat4("model", glm::mat4(1.0f));
            nanosuit.Draw(shaderDebug);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        
        delete cam_xyz_buffer;
        
        }
    #endif


        return return_value;
    }

private:

    Shader shaderGeometryPass;
    Shader shaderDebug;
    Model nanosuit;
    unsigned int gBuffer;
    unsigned int gAlbedoSpec;
    unsigned int rboDepth;

    void setup_opengl() {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
#ifndef SHOW_DEPTH
        glfwWindowHint(GLFW_VISIBLE, 0);
#endif

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(window);
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

#ifdef USE_CAM_IN_POSE
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#else
        glfwSetKeyCallback(window, key_callback);
#endif

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

        // configure g-buffer framebuffer
        // ------------------------------
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedoSpec, 0);

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glDepthMask(GL_TRUE);

    }


};

#endif