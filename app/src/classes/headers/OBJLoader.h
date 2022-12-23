#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>

#include "Struct.h"



class OBJLoader {
 public:
  OBJLoader(std::string& obj_filename);
  ~OBJLoader();

  std::string OBJLoader::ToString();
  std::vector<Vertex> GetVertices();
  std::vector<TextureCoordinate> GetTextureCoordinates();
  std::vector<Normal> GetNormals();
  std::vector<Face> GetFaces();
  std::vector<Material> GetMaterials();
  Material GetMaterialsFromFaceId(int faceId);
  Material GetMaterialsFromFace(Face& face);


  

 private:
  std::vector<Vertex> vertices_;
  std::vector<TextureCoordinate> texture_coordinates_;
  std::vector<Normal> normals_;
  std::vector<Face> faces_;
  std::vector<Material> materials_;
  
  std::map<int, std::string> face_id_to_material_map;
  std::map<Face, std::string> face_to_material_map;
  std::map<std::string, Material> material_name_to_material_map;
  
};

#endif  // OBJ_LOADER_H
