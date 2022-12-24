#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>

struct Color {
  float r; // Red component (0.0 - 1.0)
  float g; // Green component (0.0 - 1.0)
  float b; // Blue component (0.0 - 1.0)
  float a; // Alpha (transparency) component (0.0 - 1.0)

  Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0) : r(r), g(g), b(b), a(a) {}
};

struct Vertex {
  float x;
  float y;
  float z;

  Vertex(float x = 0.0, float y = 0.0, float z = 0.0) : x(x), y(y), z(z) {}
};

struct TextureCoordinate {
  float u;
  float v;
};

struct Normal {
  float x;
  float y;
  float z;

  Normal(float x = 0.0, float y = 0.0, float z = 0.0) : x(x), y(y), z(z) {}
};

struct IndexesFace {
  unsigned int v1;
  unsigned int v2;
  unsigned int v3;

  IndexesFace(){}
  IndexesFace(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) {}

  bool operator<(const IndexesFace& other) const {
    if (v1 != other.v1) return v1 < other.v1;
    if (v2 != other.v2) return v2 < other.v2;
    return v3 < other.v3;
  }
};

struct Face {
  unsigned int v1;
  unsigned int v2;
  unsigned int v3;
  unsigned int vt1;
  unsigned int vt2;
  unsigned int vt3;
  unsigned int vn1;
  unsigned int vn2;
  unsigned int vn3;

  Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3), vt1(0), vt2(0), vt3(0), vn1(0), vn2(0), vn3(0) {}

  // bool operator<(const Face& other) const {
  //   if (v1 != other.v1) return v1 < other.v1;
  //   if (v2 != other.v2) return v2 < other.v2;
  //   if (v3 != other.v3) return v3 < other.v3;
  //   if (vt1 != other.vt1) return vt1 < other.vt1;
  //   if (vt2 != other.vt2) return vt2 < other.vt2;
  //   if (vt3 != other.vt3) return vt3 < other.vt3;
  //   if (vn1 != other.vn1) return vn1 < other.vn1;
  //   if (vn2 != other.vn2) return vn2 < other.vn2;
  //   return vn3 < other.vn3;
  // }
};

struct OBJMaterial {
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

class OBJLoader {
 public:
  OBJLoader(const std::string& obj_filename);
  ~OBJLoader();

  std::string ToString();
  std::vector<Vertex> GetVertices();
  std::vector<TextureCoordinate> GetTextureCoordinates();
  std::vector<Normal> GetNormals();
  std::vector<IndexesFace> GetFaces();
  std::vector<OBJMaterial> GetMaterials();
  OBJMaterial GetMaterialsFromFaceId(int faceId);
  OBJMaterial GetMaterialsFromFace(IndexesFace& face);

 private:
  std::vector<Vertex> m_Vertices;
  std::vector<TextureCoordinate> m_TexCoords;
  std::vector<Normal> m_Normals;
  std::vector<IndexesFace> m_Faces;
  std::vector<OBJMaterial> m_Materials;
  
  std::map<int, std::string> m_Face_id_to_material_map;
  std::map<IndexesFace, std::string> m_Face_to_material_map;
  std::map<std::string, OBJMaterial> m_Material_name_to_material_map;
  
};

#endif  // OBJ_LOADER_H
