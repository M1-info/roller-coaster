#include "headers/OBJLoader.h"
#include <iostream>
#include <string>

using namespace std;

OBJLoader::OBJLoader(std::string& obj_filename) {
    std::string line;
    Material material;
    std::string current_material;
    int face_index = 0;

    #if VISUAL_STUDIO
    obj_filename = "src/assets/obj/" + obj_filename;
    #elif MINGW
    obj_filename = "app\\src\\assets\\obj\\" + obj_filename;
    #endif

    std::ifstream obj_file(obj_filename);
    if (obj_file.is_open()) {
        cout<<"Loading "<<obj_filename<<"..."<<endl;
        while (std::getline(obj_file, line)) {
            if (line.substr(0, 2) == "v ") {
                // Vertex
                Vertex vertex;
                sscanf_s(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                vertices_.push_back(vertex);
            } else if (line.substr(0, 3) == "vt ") {
                // Texture coordinate
                TextureCoordinate tex_coord;
                sscanf_s(line.c_str(), "vt %f %f", &tex_coord.u, &tex_coord.v);
                texture_coordinates_.push_back(tex_coord);
            } else if (line.substr(0, 3) == "vn ") {
                // Normal
                Normal normal;
                sscanf_s(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                normals_.push_back(normal);
            } else if (line.substr(0, 2) == "f ") {
                // Face
                Face face;
                int matches = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &face.v1, &face.vt1, &face.vn1, &face.v2, &face.vt2, &face.vn2, &face.v3, &face.vt3, &face.vn3);
                if (matches != 9) {
                    // Normals are optional
                    matches = sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &face.v1, &face.vt1, &face.v2, &face.vt2, &face.v3, &face.vt3);
                    if (matches != 6) {
                        // Texture coordinates and normals are optional
                        matches = sscanf_s(line.c_str(), "f %d %d %d", &face.v1, &face.v2, &face.v3);
                    }
                    if (matches == 3) {
                        // Only vertices are specified
                        sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &face.v1, &face.vn1, &face.v2, &face.vn2, &face.v3, &face.vn3);
                        face.vt1 = 0;
                        face.vt2 = 0;
                        face.vt3 = 0;
                    }
                }
                faces_.push_back(face);

                face_id_to_material_map[face_index] = current_material;
                face_to_material_map[face] = current_material;
                face_index++;
            } else if (line.substr(0, 7) == "usemtl ") {
                // Material
                current_material = line.substr(7);
            } else if (line.substr(0, 7) == "mtllib ") {
                // Material library
                std::string mtl_filename = line.substr(7);

                #if VISUAL_STUDIO
                mtl_filename = "src/assets/obj/" + mtl_filename;
                #elif MINGW
                mtl_filename = "app\\src\\assets\\obj\\" + mtl_filename;
                #endif

                std::ifstream mtl_file(mtl_filename);
                if (mtl_file.is_open()) {
                    cout<<"Loading "<< mtl_filename<<"..."<<endl;
                    int i = 0;
                    while (std::getline(mtl_file, line)) {
                        if (line.substr(0, 7) == "newmtl ") {
                        cout<<"Loading "<< line<<"..."<<endl;
                        if(i!=0){
                            materials_.push_back(material);
                            material_name_to_material_map[material.name] = material;
                        }
                        i++;
                        // New material
                        material = Material();
                        material.name = line.substr(7);
                        } else if (line.substr(0, 3) == "Ka ") {
                        // Ambient color
                        sscanf_s(line.c_str(), "Ka %f %f %f", &material.ambient_color.r, &material.ambient_color.g, &material.ambient_color.b);
                        } else if (line.substr(0, 3) == "Kd ") {
                        // Diffuse color
                        sscanf_s(line.c_str(), "Kd %f %f %f", &material.diffuse_color.r, &material.diffuse_color.g, &material.diffuse_color.b);
                        } else if (line.substr(0, 3) == "Ks ") {
                        // Specular color
                        sscanf_s(line.c_str(), "Ks %f %f %f", &material.specular_color.r, &material.specular_color.g, &material.specular_color.b);
                        } else if (line.substr(0, 3) == "Ke ") {
                        // Emissive color
                        sscanf_s(line.c_str(), "Ke %f %f %f", &material.emissive_color.r, &material.emissive_color.g, &material.emissive_color.b);
                        } else if (line.substr(0, 2) == "Ns") {
                        // Shininess
                        sscanf_s(line.c_str(), "Ns %f", &material.shininess);
                        } else if (line.substr(0, 2) == "Ni") {
                        // Optical density
                        sscanf_s(line.c_str(), "Ni %f", &material.optical_density);
                        } else if (line.substr(0, 2) == "d ") {
                        // Transparency
                        sscanf_s(line.c_str(), "d %f", &material.transparency);
                        } else if (line.substr(0, 5) == "illum") {
                        // Transparency
                        sscanf_s(line.c_str(), "illum %d", &material.illumination_model);
                        } else if (line.substr(0, 6) == "map_Kd") {
                        // Diffuse texture map
                        material.diffuse_texture_map = line.substr(7);
                        }
                        
                    }
                    materials_.push_back(material);
                }
                else{
                    std::cout << "Could not open file " << mtl_filename << std::endl;
                }
                mtl_file.close();
            }
        
        }
        obj_file.close();
    }
    else{
        cout << "Could not open file " << obj_filename << endl;
    }
}




std::vector<Vertex> OBJLoader::GetVertices() {
  return vertices_;
}

std::vector<TextureCoordinate> OBJLoader::GetTextureCoordinates() {
  return texture_coordinates_;
}

std::vector<Normal> OBJLoader::GetNormals() {
  return normals_;
}

std::vector<Face> OBJLoader::GetFaces() {
  return faces_;
}

std::vector<Material> OBJLoader::GetMaterials() {
  return materials_;
}

OBJLoader::~OBJLoader() {
  // Clear the vectors
  vertices_.clear();
  texture_coordinates_.clear();
  normals_.clear();
  faces_.clear();
  materials_.clear();
}

std::string OBJLoader::ToString() {
    std::stringstream str;
    str << "Vertices:\n";
    for (const auto& v : vertices_) {
        str << "  " << v.x << ", " << v.y << ", " << v.z << "\n";
    }
    str << "Texture coordinates:\n";
    for (const auto& tc : texture_coordinates_) {
        str << "  " << tc.u << ", " << tc.v << "\n";
    }
    str << "Normals:\n";
    for (const auto& n : normals_) {
        str << "  " << n.x << ", " << n.y << ", " << n.z << "\n";
    }
    str << "Faces:\n";
    for (const auto& f : faces_) {
        str << "  " << f.v1 << ", " << f.v2 << ", " << f.v3 << ", " << f.vt1 << ", " << f.vt2 << ", " << f.vt3 << ", " << f.vn1 << ", " << f.vn2 << ", " << f.vn3 << "\n";
    }
    str << "Materials:\n";
    for (const auto& m : materials_) {
        str << "  Name: " << m.name << "\n"
            << "  Ambient color: " << m.ambient_color.r << ", " << m.ambient_color.g << ", " << m.ambient_color.b << "\n"
            << "  Diffuse color: " << m.diffuse_color.r << ", " << m.diffuse_color.g << ", " << m.diffuse_color.b << "\n"
            << "  Specular color: " << m.specular_color.r << ", " << m.specular_color.g << ", " << m.specular_color.b << "\n"
            << "  Emissive color: " << m.emissive_color.r << ", " << m.emissive_color.g << ", " << m.emissive_color.b << "\n"
            << "  Shininess: " << m.shininess << "\n"
            << "  Transparency: " << m.transparency << "\n"
            << "  Optical density: " << m.optical_density << "\n"
            << "  illumination model: " << m.illumination_model << "\n"
            << "  Diffuse texture map: " << m.diffuse_texture_map << "\n";
    }
    return str.str();
}


Material OBJLoader::GetMaterialsFromFaceId(int face_id) {
  // Look up the material name for the face index
  auto it = face_id_to_material_map.find(face_id);
  if (it != face_id_to_material_map.end()) {
    std::string material_name = it->second;
    // Look up the full Material object for the material name
    auto it2 = material_name_to_material_map.find(material_name);
    if (it2 != material_name_to_material_map.end()) {
      return it2->second;
    } else {
      // The material name was not found in the map
      return Material();
    }
  } else {
    // The face index was not found in the map
    return Material();
  }
}

Material OBJLoader::GetMaterialsFromFace(Face& face) {
  // Look up the material name for the face
  auto it = face_to_material_map.find(face);
  if (it != face_to_material_map.end()) {
    std::string material_name = it->second;
    // Look up the full Material object for the material name
    auto it2 = material_name_to_material_map.find(material_name);
    if (it2 != material_name_to_material_map.end()) {
      return it2->second;
    } else {
      // The material name was not found in the map
      return Material();
    }
  } else {
    // The face was not found in the map
    return Material();
  }
}

