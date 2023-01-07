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
  inline std::vector<Object> GetObjects() const { return m_Objects; }
  inline std::vector<glm::vec3> GetVertices() const { return m_Vertices; }
  inline std::vector<glm::vec2> GetTextureCoordinates() const { return m_TexCoords; }
  inline std::vector<glm::vec3> GetNormals() const { return m_Normals; }
  inline std::vector<IndexesFace> GetFaces() const { return m_Faces; }
  inline std::vector<OBJMaterial> GetMaterials() const { return m_Materials; }
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
