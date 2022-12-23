#ifndef STRUCT_H
#define STRUCT_H

#include <string>

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

  bool operator<(const Face& other) const {
    if (v1 != other.v1) return v1 < other.v1;
    if (v2 != other.v2) return v2 < other.v2;
    if (v3 != other.v3) return v3 < other.v3;
    if (vt1 != other.vt1) return vt1 < other.vt1;
    if (vt2 != other.vt2) return vt2 < other.vt2;
    if (vt3 != other.vt3) return vt3 < other.vt3;
    if (vn1 != other.vn1) return vn1 < other.vn1;
    if (vn2 != other.vn2) return vn2 < other.vn2;
    return vn3 < other.vn3;
  }
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
  int illumination_model;
  std::string diffuse_texture_map;
};


#endif 