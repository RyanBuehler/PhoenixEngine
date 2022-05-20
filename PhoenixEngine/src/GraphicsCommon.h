#pragma once
#include "glm/glm.hpp"
#include "GLEW/glew.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Graphics
{
	enum class DataUsage
	{
		STATIC = 0,
		DYNAMIC,
		STREAM
	};

	static void retrieve_shader_log(const GLint ShaderId, string& Log) noexcept
	{
		int logLength;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &logLength);
    const auto shaderLog = new char[logLength];
		glGetShaderInfoLog(ShaderId, logLength, &logLength, shaderLog);
		Log.clear();
		Log = shaderLog;
		delete[] shaderLog;
	}

	static void retrieve_program_log(const GLuint ProgramId, string& Log) noexcept
	{
		int logLength;
		glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &logLength);
    const auto programLog = new char[logLength];
		glGetProgramInfoLog(ProgramId, logLength, &logLength, programLog);
		Log.clear();
		Log = programLog;
		delete[] programLog;
	}
}

namespace Shader
{
	enum class Type : unsigned
	{
		UNKNOWN = 0u,
		VERTEX,
		FRAGMENT
	};

	enum class Vertex : unsigned
	{
		UNKNOWN = 0u,
		SKYBOX,
		BLINN_PHONG,
		DEBUG,
		COUNT
	};

	enum class Fragment : unsigned
	{
		UNKNOWN = 0u,
		SKYBOX,
		BLINN_PHONG,
		DEBUG,
		COUNT
	};
}

namespace Normals
{
	enum class Type : unsigned
	{
		VERTEX = 0u,
		TRIANGLE,
		SURFACE,
		COUNT
	};
}

namespace UV
{
	enum class Generation : unsigned
	{
		CUSTOM = 0u,
		SPHERICAL,
		CYLINDRICAL,
		PLANAR
	};
}

namespace Error
{
	constexpr unsigned INVALID_INDEX = numeric_limits<unsigned>::max();
	namespace Handle
	{
		constexpr GLuint INVALID_HANDLE = numeric_limits<GLuint>::max();
	}

	namespace Context
	{
		constexpr unsigned INVALID_CONTEXT = numeric_limits<GLint>::max();
	}

	namespace OpenGL
	{
		constexpr unsigned PROGRAM_ERROR = numeric_limits<unsigned>::max();
	}
}