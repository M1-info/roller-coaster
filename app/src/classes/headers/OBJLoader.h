#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>

struct Color {
  float r; // Red component (0.0 - 1.0)
  float g; // Green component (0.0 - 1.0)
  float b; // Blue component (0.0 - 1.0)
  float a; // Alpha (transparency) component (0.0 - 1.0)

  // Constructor
  Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0) : r(r), g(g), b(b), a(a) {}
};

struct Vertex {
  float x;
  float y;
  float z;
};

struct TextureCoordinate {
  float u;
  float v;
};

struct Normal {
  float x;
  float y;
  float z;
};

struct Face {
  int v1;
  int v2;
  int v3;
  int vt1;
  int vt2;
  int vt3;
  int vn1;
  int vn2;
  int vn3;
};

struct Material {
  std::string name;
  Color ambient_color;
  Color diffuse_color;
  Color specular_color;
  Color emissive_color;
  float shininess;
  float transparency;
  float optical_density;
  std::string diffuse_texture_map;
};

class OBJLoader {
 public:
  OBJLoader(const std::string& obj_filename);
  ~OBJLoader();


  std::vector<Vertex> GetVertices();
  std::vector<TextureCoordinate> GetTextureCoordinates();
  std::vector<Normal> GetNormals();
  std::vector<Face> GetFaces();
  std::vector<Material> GetMaterials();

 private:
  std::vector<Vertex> vertices_;
  std::vector<TextureCoordinate> texture_coordinates_;
  std::vector<Normal> normals_;
  std::vector<Face> faces_;
  std::vector<Material> materials_;
};

#endif  // OBJ_LOADER_H
