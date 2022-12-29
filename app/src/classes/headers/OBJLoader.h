#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>

#include <glm/glm.hpp>

struct Color
{
  float r; // Red component (0.0 - 1.0)
  float g; // Green component (0.0 - 1.0)
  float b; // Blue component (0.0 - 1.0)
  float a; // Alpha (transparency) component (0.0 - 1.0)

  Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0) : r(r), g(g), b(b), a(a) {}
};

struct IndexesFace
{
  unsigned int vertices[3];
  unsigned int texCoords[3];
  unsigned int normals[3];
  // unsigned int v1;
  // unsigned int v2;
  // unsigned int v3;

  IndexesFace() {}

  bool operator<(const IndexesFace &other) const
  {
    if (vertices[0] != other.vertices[0])
      return vertices[0] < other.vertices[0];
    if (vertices[1] != other.vertices[1])
      return vertices[1] < other.vertices[1];
    return vertices[2] < other.vertices[2];
  }
};

struct OBJMaterial
{
  std::string name;
  Color ambient_color;
  Color diffuse_color;
  Color specular_color;
  Color emissive_color;
  float shininess;
  float transparency;
  float optical_density;
  int illumination_model;
  std::string diffuse_texture_map;
};

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
