//------------------------------------------------------------------------------
// File:    pch.h
// Author:  Ryan Buehler
// Created: September 3, 2021
// Desc:    Pre-Compiled Header for the Phoenix Engine
//------------------------------------------------------------------------------
#pragma once

#define PE_VERSION "v1.0"

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
#include <iostream>
#include <fstream>

// Data Structures & Pointers
#include <memory>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <set>

// Functional
#include <functional>

// Type
#include <type_traits>

// Utility Functions
#include <algorithm>
#include <limits>

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
using std::list;
using std::forward_list;
using std::set;

// Functional
using std::function;

// Type
using std::is_same;

// Utility Functions
using std::clamp;
using std::numeric_limits;
using std::pair;
using std::make_pair;

// ----- MACROS -----
#define SSIO std::ios_base::app | std::ios_base::out