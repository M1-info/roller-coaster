#pragma once
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include "Object.h"
#include <glm/glm.hpp>



class OBJLoader
{
public:
  OBJLoader(const std::string &obj_filename);
  ~OBJLoader();

  std::string ToString();
  std::vector<glm::vec3> GetVertices();
  std::vector<glm::vec2> GetTextureCoordinates();
  std::vector<glm::vec3> GetNormals();
  std::vector<IndexesFace> GetFaces();
  std::vector<OBJMaterial> GetMaterials();
  OBJMaterial GetMaterialsFromFaceId(int faceId);
  OBJMaterial GetMaterialsFromFace(IndexesFace &face);

private:

  std::vector<Object> m_Objects;
  std::vector<glm::vec3> m_Vertices;
  std::vector<glm::vec2> m_TexCoords;
  std::vector<glm::vec3> m_Normals;
  std::vector<IndexesFace> m_Faces;
  std::vector<OBJMaterial> m_Materials;

  std::map<int, std::string> m_Face_id_to_material_map;
  std::map<IndexesFace, std::string> m_Face_to_material_map;
  std::map<std::string, OBJMaterial> m_Material_name_to_material_map;
};

#endif // OBJ_LOADER_H
