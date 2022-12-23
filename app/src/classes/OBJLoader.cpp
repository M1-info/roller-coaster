#include "headers/OBJLoader.h"

#include <fstream>
#include <string>

OBJLoader::OBJLoader(const std::string& obj_filename) {
    std::string line;
    Material material;
    std::string current_material;

    std::ifstream obj_file(obj_filename);
    if (obj_file.is_open()) {
        while (std::getline(obj_file, line)) {
            if (line.substr(0, 2) == "v ") {
                // Vertex
                Vertex vertex;
                sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                vertices_.push_back(vertex);
            } else if (line.substr(0, 3) == "vt ") {
                // Texture coordinate
                TextureCoordinate tex_coord;
                sscanf(line.c_str(), "vt %f %f", &tex_coord.u, &tex_coord.v);
                texture_coordinates_.push_back(tex_coord);
            } else if (line.substr(0, 3) == "vn ") {
                // Normal
                Normal normal;
                sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
                normals_.push_back(normal);
            } else if (line.substr(0, 2) == "f ") {
                // Face
                Face face;
                int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &face.v1, &face.vt1, &face.vn1, &face.v2, &face.vt2, &face.vn2, &face.v3, &face.vt3, &face.vn3);
                if (matches != 9) {
                    // Normals are optional
                    matches = sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &face.v1, &face.vt1, &face.v2, &face.vt2, &face.v3, &face.vt3);
                    if (matches != 6) {
                        // Texture coordinates and normals are optional
                        matches = sscanf(line.c_str(), "f %d %d %d", &face.v1, &face.v2, &face.v3);
                    }
                    if (matches == 3) {
                        // Only vertices are specified
                        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &face.v1, &face.vn1, &face.v2, &face.vn2, &face.v3, &face.vn3);
                        face.vt1 = 0;
                        face.vt2 = 0;
                        face.vt3 = 0;
                    }
                }
                faces_.push_back(face);
            } else if (line.substr(0, 7) == "usemtl ") {
                // Material
                current_material = line.substr(7);
            } else if (line.substr(0, 7) == "mtllib ") {
                // Material library
                std::string mtl_filename = line.substr(7);
                std::ifstream mtl_file(mtl_filename);
                if (mtl_file.is_open()) {
                while (std::getline(mtl_file, line)) {
                        if (line.substr(0, 7) == "newmtl ") {
                        // New material
                        material = Material();
                        material.name = line.substr(7);
                        } else if (line.substr(0, 3) == "Ka ") {
                        // Ambient color
                        sscanf(line.c_str(), "Ka %f %f %f", &material.ambient_color.r, &material.ambient_color.g, &material.ambient_color.b);
                        } else if (line.substr(0, 3) == "Kd ") {
                        // Diffuse color
                        sscanf(line.c_str(), "Kd %f %f %f", &material.diffuse_color.r, &material.diffuse_color.g, &material.diffuse_color.b);
                        } else if (line.substr(0, 3) == "Ks ") {
                        // Specular color
                        sscanf(line.c_str(), "Ks %f %f %f", &material.specular_color.r, &material.specular_color.g, &material.specular_color.b);
                        } else if (line.substr(0, 3) == "Ke ") {
                        // Emissive color
                        sscanf(line.c_str(), "Ke %f %f %f", &material.emissive_color.r, &material.emissive_color.g, &material.emissive_color.b);
                        } else if (line.substr(0, 2) == "Ns") {
                        // Shininess
                        sscanf(line.c_str(), "Ns %f", &material.shininess);
                        } else if (line.substr(0, 2) == "Ni") {
                        // Optical density
                        sscanf(line.c_str(), "Ni %f", &material.optical_density);
                        } else if (line.substr(0, 2) == "d ") {
                        // Transparency
                        sscanf(line.c_str(), "d %f", &material.transparency);
                        } else if (line.substr(0, 6) == "map_Kd") {
                        // Diffuse texture map
                        material.diffuse_texture_map = line.substr(7);
                        }
                    }
                    materials_.push_back(material);
                }
            }
        obj_file.close();
        }
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