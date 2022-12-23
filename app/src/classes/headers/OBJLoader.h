#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#include "Struct.h"

class OBJLoader {
 public:
  OBJLoader(const std::string& obj_filename);
  ~OBJLoader();

  std::string OBJLoader::ToString();
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
