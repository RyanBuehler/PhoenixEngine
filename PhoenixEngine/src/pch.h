#pragma once

// Includes
#include "Logger.h"
#include "Utility.h"
#include "Paths.h"

// ----- STL -----
// Strings
#include <string>
#include <sstream>
#include <cstring>

// Streams
#include <fstream>

// Data Structures & Pointers
#include <memory>
#include <vector>
#include <array>

// Utility Functions
#include <algorithm>

// ----- USING -----
// Strings
using std::string;
using std::stringstream;
using std::endl;

// Streams
using std::ifstream;

// Data Structures & Pointers
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::vector;
using std::array;

// Utility Functions
using std::clamp;

// ----- MACROS -----
#define SSIO std::ios_base::app | std::ios_base::out