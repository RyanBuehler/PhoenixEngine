//
// Created by pushpak on 4/5/18.
//

#ifndef SIMPLE_SCENE_OBJREADER_H
#define SIMPLE_SCENE_OBJREADER_H

#include <string>
#include <fstream>
#include <vector>

// for OpenGL datatypes
#include <GLEW/glew.h>
#include <glm/glm.hpp>

class Mesh;

class OBJReader
{

public:
  OBJReader();
  virtual ~OBJReader();

  // initialize the data
  void initData();

  // Read data from a file
  enum class ReadMethod { LINE_BY_LINE, BLOCK_IO };
  double ReadOBJFile(std::string filepath,
    Mesh* pMesh,
    ReadMethod r = ReadMethod::LINE_BY_LINE,
    GLboolean bFlipNormals = false);

private:

  // Read OBJ file line by line
  int ReadOBJFile_LineByLine(std::string filepath);

  // Read the OBJ file in blocks -- works for files smaller than 1GB
  int ReadOBJFile_BlockIO(std::string filepath);

  // Parse individual OBJ record (one line delimited by '\n')
  void ParseOBJRecord(char* buffer, glm::vec3& min, glm::vec3& max);

  // data members
  Mesh* _currentMesh;
};


#endif //SIMPLE_SCENE_OBJREADER_H
