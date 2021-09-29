//
// Created by pushpak on 4/5/18.
//
#include "pch.h"
#include <iostream>
#include <cstring>
#define GLM_ENABLE_EXPERIMENTAL
#include "GLEW/glew.h"
#include <cfloat>
#include <chrono>
#include <set>
#include "OBJReader.h"
#include "Mesh.h"

OBJReader::OBJReader()
{
  initData();
}

OBJReader::~OBJReader()
{
  initData();
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
void OBJReader::initData()
{
  _currentMesh = nullptr;
}


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
double OBJReader::ReadOBJFile(std::string filepath, Mesh* pMesh,
  OBJReader::ReadMethod r, GLboolean bFlipNormals)
{
  int rFlag = -1;

  if (pMesh)
    _currentMesh = pMesh;
  else
    return rFlag;

  //    clock_t  startTime, endTime;

  auto startTime = std::chrono::high_resolution_clock::now();

  switch (r)
  {
  case OBJReader::ReadMethod::LINE_BY_LINE:
    rFlag = ReadOBJFile_LineByLine(filepath);
    break;

  case OBJReader::ReadMethod::BLOCK_IO:
    rFlag = ReadOBJFile_BlockIO(filepath);
    break;

  default:
    std::cout << "Unknown value for OBJReader::ReadMethod in function ReadObjFile." << std::endl;
    std::cout << "Quitting ..." << std::endl;
    rFlag = -1;
    break;
  }

  auto endTime = std::chrono::high_resolution_clock::now();

  double timeDuration = std::chrono::duration< double, std::milli >(endTime - 
    startTime).count();

  std::cout << "OBJ file read in "
    << timeDuration
    << "  milli seconds." << std::endl;


  // Now calculate vertex normals
  //_currentMesh->calcVertexNormals(bFlipNormals);
  //_currentMesh->calcUVs(Mesh::UVType::CYLINDRICAL_UV);

  return timeDuration;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
int OBJReader::ReadOBJFile_LineByLine(std::string filepath)
{
  int rFlag = -1;
  glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
  glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

  std::ifstream  inFile;
  inFile.open("res/models/" + filepath);

  if (inFile.bad() || inFile.eof() || inFile.fail())
    return rFlag;

  while (!inFile.eof())
  {
    char buffer[256] = "\0";
    inFile.getline(buffer, 256, '\n');

    // Use only for debugging purpose
//        std::cout << buffer << std::endl;
    ParseOBJRecord(buffer, min, max);
  }

  //_currentMesh->boundingBox[0] = min;
  //_currentMesh->boundingBox[1] = max;

  return rFlag;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
// Read the OBJ file in blocks -- works for files smaller than 1GB
int OBJReader::ReadOBJFile_BlockIO(std::string filepath)
{
  int rFlag = -1;
  long int OneGBinBytes = 1024 * 1024 * 1024 * sizeof(char);

  glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
  glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);


  // Check the file size, if > 1 GB, abort
  std::ifstream inFile(filepath, std::ifstream::in | std::ifstream::binary);

  if (inFile.bad() || inFile.eof())
    return rFlag;

  char* fileContents = NULL;
  long int count = 0;

  // get the file size
  inFile.seekg(0, std::ifstream::end);
  count = static_cast<long>(inFile.tellg());
  inFile.seekg(0, std::ifstream::beg);

  if (count <= 0 || count >= OneGBinBytes)
  {
    std::cout << " Error reading file " << filepath << std::endl;
    std::cout << "File size reported as : " << count << " bytes." << std::endl;
  }
  else if (count > 0)
  {
    const char* delims = "\n\r";
    fileContents = (char*)malloc(sizeof(char) * (static_cast<size_t>(count) + 1));
    assert(fileContents);
    inFile.read(fileContents, count);
    fileContents[count] = '\0';

    rFlag = 0;

    // Now parse the obj file
    char* currPtr = fileContents;
    assert(currPtr);
    char* token = strpbrk(currPtr, delims);

    while (token != nullptr)
    {
      int numChars = static_cast<int>(token - currPtr);
      char ObjLine[256];
      assert(currPtr != nullptr);
      strncpy_s(ObjLine, currPtr, numChars);
      ObjLine[numChars] = '\0';

      ParseOBJRecord(ObjLine, min, max);

      currPtr = token + 1;
      token = strpbrk(currPtr, delims);
    }

    free(fileContents);

    //_currentMesh->boundingBox[0] = min;
    //_currentMesh->boundingBox[1] = max;
  }

  return rFlag;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
void OBJReader::ParseOBJRecord(char* buffer, glm::vec3& min, glm::vec3& max)
{
  const char* delims = " \r\n\t";
  GLfloat x, y, z;

  GLfloat   temp;
  GLuint    firstIndex, secondIndex, thirdIndex;
  
  char* token = NULL;
  char* next_token = NULL;
  token = strtok_s(buffer, delims, &next_token);
  //char* token = strtok(buffer, delims);

  // account for empty lines
  if (token == nullptr)
    return;

  switch (token[0])
  {
  case 'v':
    // vertex coordinates
    if (token[1] == '\0')
    {
      token = strtok_s(nullptr, delims, &next_token);
      temp = static_cast<GLfloat&&>(atof(token));
      if (min.x > temp)
        min.x = temp;
      if (max.x <= temp)
        max.x = temp;
      x = temp;

      token = strtok_s(nullptr, delims, &next_token);
      temp = static_cast<GLfloat&&>(atof(token));
      if (min.y > temp)
        min.y = temp;
      if (max.y <= temp)
        max.y = temp;
      y = temp;

      token = strtok_s(nullptr, delims, &next_token);
      temp = static_cast<GLfloat&&>(atof(token));
      if (min.z > temp)
        min.z = temp;
      if (max.z <= temp)
        max.z = temp;
      z = temp;

      _currentMesh->AddVertex(x, y, z);
    }
    // vertex normals
    else if (token[1] == 'n')
    {
      glm::vec3 vNormal(0.f);

      token = strtok_s(nullptr, delims, &next_token);
      if (token == nullptr)
        break;

      vNormal[0] = static_cast<GLfloat&&>(atof(token));

      token = strtok_s(nullptr, delims, &next_token);
      if (token == nullptr)
        break;

      vNormal[1] = static_cast<GLfloat&&>(atof(token));

      token = strtok_s(nullptr, delims, &next_token);
      if (token == nullptr)
        break;

      vNormal[2] = static_cast<GLfloat&&>(atof(token));

      _currentMesh->AddNormal(glm::normalize(vNormal));
    }

    break;

  case 'f':
    token = strtok_s(nullptr, delims, &next_token);
    if (token == nullptr)
      break;
    firstIndex = static_cast<unsigned int&&>(atoi(token) - 1);

    token = strtok_s(nullptr, delims, &next_token);
    if (token == nullptr)
      break;
    secondIndex = static_cast<unsigned int&&>(atoi(token) - 1);

    token = strtok_s(nullptr, delims, &next_token);
    if (token == nullptr)
      break;
    thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

    // push back first triangle
    _currentMesh->AddTriangle(firstIndex, secondIndex, thirdIndex);

    token = strtok_s(nullptr, delims, &next_token);

    while (token != nullptr)
    {
      secondIndex = thirdIndex;
      thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

      _currentMesh->AddTriangle(firstIndex, secondIndex, thirdIndex);

      token = strtok_s(nullptr, delims, &next_token);
    }

    break;

  case '#':
  default:
    break;
  }

  return;
}
