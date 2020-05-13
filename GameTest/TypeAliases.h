#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template <typename T> using UniqueVector = std::vector<std::unique_ptr<T>>;
using uint = unsigned int;
