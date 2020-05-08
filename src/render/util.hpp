#ifndef UTIL_HPP
#define UTIL_HPP

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

inline Eigen::Vector3f
getCameraTranslation(const boost::property_tree::ptree &pt) {
    // Read in camera translation
    float tr[3];
    int i = 0;
    boost::property_tree::ptree::const_iterator end = pt.get_child("camera_location").end();
    for (boost::property_tree::ptree::const_iterator it = pt.get_child("camera_location").begin(); it != end; ++it, ++i) {
        tr[i] = it->second.get_value<float>();
    }
    // Eigen::Affine3f translation(Eigen::Translation3f(tr[0], tr[1], tr[2]));
    return {tr[0], tr[1], tr[2]};
}

/** Gets the camera rotation from a pose file ptree and returns it**/
inline Eigen::Quaternionf
getCameraRotation(const boost::property_tree::ptree &pt) {
    // Read in the camera euler angles
    int i = 0;
    float euler_angles[3];
    boost::property_tree::ptree::const_iterator end = pt.get_child("final_camera_rotation").end();
    for (boost::property_tree::ptree::const_iterator it = pt.get_child("final_camera_rotation").begin(); it != end; ++it, ++i) {
        euler_angles[i] = it->second.get_value<float>();
        // std::cout << "Angle " << i << ": " << euler_angles[i] << std::endl;
    }
    //Roll pitch and yaw in Radians
    float roll = euler_angles[2], pitch = euler_angles[1], yaw = euler_angles[0];
    // std::cout << roll << ", " << pitch <<  ", " << yaw << std::endl;
    Eigen::Quaternionf q;
    q = Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitZ())
        * Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
        * Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitX());
    return q;
}

/** Load in the json pose files into a boost::ptree **/
inline boost::property_tree::ptree
loadPoseFile(const std::string &json_filename) {
    // Read in view_dict
    boost::property_tree::ptree pt;
    std::ifstream in(json_filename);
    std::stringstream buffer;
    buffer << in.rdbuf();
    read_json(buffer, pt);
    return pt;
}
inline Eigen::Matrix3f
getCameraIntrinsics(const boost::property_tree::ptree &pt) {
    // Read in camera Intrinsics
    Eigen::Matrix3f intrinsics;
    // BOOST_FOREACH (boost::property_tree::ptree::value_type &v, pt.get_child('camera_k_matrix')) {
    //     boost::property_tree::ptree subtree = v.second;
    //     BOOST_FOREACH (boost::property_tree::ptree::value_type &vs, subtree) {
    //         intrinsics << vs.second.get_value<float>();
    //     }
    // }

    int i = 0;
    boost::property_tree::ptree::const_iterator end = pt.get_child("camera_k_matrix").end();
    for (boost::property_tree::ptree::const_iterator it = pt.get_child("camera_k_matrix").begin(); it != end; ++it, ++i) {
        boost::property_tree::ptree subtree = it->second;
        int j = 0;
        boost::property_tree::ptree::const_iterator send = subtree.end();
        for (boost::property_tree::ptree::const_iterator sit = subtree.begin(); sit != send; ++sit, ++j) {
            intrinsics(i, j) = sit->second.get_value<float>();
        }
    }
    return intrinsics;
}

// inline void fix_obj_rotation(Model &obj) {
//     // TODO(ybbbbt): bug here
//     Eigen::Matrix3f rot;
//     // clang-format off
//     rot << 1, 0, 0,
//         0, 0, 1,
//         0,-1, 0;
//     // clang-format on
//     rot = rot.inverse().eval();
//     glm::mat3 transform = glm::make_mat3(rot.data());
//     auto &meshes = obj.meshes;
//     for (auto &mesh: meshes) {
//         for (auto &vertex: mesh.vertices) {
//             vertex.Position = transform * vertex.Position;
//         }
//         mesh.SetupMeshAfterInitialize();
//     }
// }

// ref: https://jinyu.li/2017/02/09/intrinsic-to-viewproj/
inline glm::mat4 convert2projection_matrix(Eigen::Matrix3f intrinsic, float w, float h,
                                    float n = 0.01, float f = 300) {
    float fx = intrinsic(0, 0), fy = intrinsic(1, 1);
    float cx = intrinsic(0, 2), cy = intrinsic(1, 2);
    Eigen::Matrix4f proj_mat;
    proj_mat << 
        2*fx/w,           0,     (2*cx-w)/w,               0,
        0,           2*fy/h,     (2*cy-h)/h,               0,
        0,                0,    (f+n)/(f-n),    -2*f*n/(f-n),
        0,                0,              1,               0;
    glm::mat4 projection_matrix = glm::make_mat4(proj_mat.data());
    return projection_matrix;
}

inline glm::mat4 convert2view_matrix(Eigen::Quaternionf rotation, Eigen::Vector3f translation) {
    // Eigen::Vector3f slam_up = {0, -1, 0};
    // Eigen::Vector3f slam_forward = {0, 0, 1};
    Eigen::Vector3f slam_up = {0, -1, 0};
    Eigen::Vector3f slam_forward = {0, 0, 1};
    slam_up = rotation * slam_up;
    slam_forward = rotation * slam_forward;
    glm::vec3 position = glm::make_vec3(translation.data());
    glm::vec3 direction = glm::make_vec3(slam_forward.data());
    glm::vec3 up = glm::make_vec3(slam_up.data());
    glm::mat4 view_matrix = glm::lookAt(position, position+direction, up);
    return view_matrix;
}

inline void print_progress_bar(float progress) {
    int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}



#endif