#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

struct IndexesFace
{
    unsigned int vertices[3];
    unsigned int texCoords[3];
    unsigned int normals[3];

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

struct Color
{
    float r; // Red component (0.0 - 1.0)
    float g; // Green component (0.0 - 1.0)
    float b; // Blue component (0.0 - 1.0)
    float a; // Alpha (transparency) component (0.0 - 1.0)

    Color(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0) : r(r), g(g), b(b), a(a) {}
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

class Object
{
public:

    Object() : vertices(), texCoords(), normals(), faces(), material(OBJMaterial()) {}
 
    Object(const std::vector<glm::vec3> &vertices,
         const std::vector<glm::vec2> &texCoords,
         const std::vector<glm::vec3> &normals,
         const std::vector<IndexesFace> &faces,
         const OBJMaterial &material)
        : vertices(vertices), texCoords(texCoords), normals(normals), faces(faces), material(material) {}


    void AddVertex(const glm::vec3 &vertex) { vertices.push_back(vertex); }
    void AddTextureCoordinate(const glm::vec2 &texCoord) { texCoords.push_back(texCoord); }
    void AddNormal(const glm::vec3 &normal) { normals.push_back(normal); }
    void AddFace(const IndexesFace &face) { faces.push_back(face); }
    void SetMaterial(const OBJMaterial &material) { this->material = material; }
    std::vector<glm::vec3> GetVertices() const { return vertices; }
    std::vector<glm::vec2> GetTextureCoordinates() const { return texCoords; }
    std::vector<glm::vec3> GetNormals() const { return normals; }
    std::vector<IndexesFace> GetFaces() const { return faces; }
    OBJMaterial GetMaterial() const { return material; }

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<IndexesFace> faces;
    OBJMaterial material;
};

#endif