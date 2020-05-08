#ifndef SC_PROJECTION_HPP
#define SC_PROJECTION_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "util.hpp"
#include "gl_renderer.hpp"

void scene_coordinate_projection(
    const std::string &shader_dir,
    const std::string &shader_name,
    const std::string &obj_path,
    const std::string &base_dir, 
    const std::string &out_dir,
    const std::string &intrinsic_fn
);

#endif