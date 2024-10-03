#ifndef TESTCASES_HPP
#define TESTCASES_HPP

// From SFML
#include "SFML/Graphics.hpp"

// Img converter and rgb converter
#include "img/ImgConverter.hpp"

// Creates a simple texture and displays it in a titlebar window.
int render_simple(std::string path);

// Convert image
int test_ascii_conversion(std::string path);

#endif
