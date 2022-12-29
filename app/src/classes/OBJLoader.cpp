#include "headers/OBJLoader.h"

OBJLoader::OBJLoader(const std::string &obj_filename)
{
  std::string line;
  OBJMaterial material;
  std::string current_material;
  int face_index = 0;

  std::string obj_filepath;

#if VISUAL_STUDIO
  obj_filepath = "src/assets/obj/" + obj_filename;
#elif MINGW
  obj_filepath = "app\\src\\assets\\obj\\" + obj_filename;
#endif

  std::ifstream obj_file(obj_filepath);

  if (!obj_file.is_open())
  {
    std::cerr << "Error: Could not open " << obj_filename << std::endl;
    return;
  }

  while (std::getline(obj_file, line))
  {
    if (line.substr(0, 2) == "v ")
    {
      // glm::vec3
      glm::vec3 vertex;
      sscanf_s(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      m_Vertices.push_back(vertex);
    }
    else if (line.substr(0, 3) == "vt ")
    {
      // Texture coordinate
      glm::vec2 tex_coord;
      sscanf_s(line.c_str(), "vt %f %f", &tex_coord.x, &tex_coord.y);
      m_TexCoords.push_back(tex_coord);
    }
    else if (line.substr(0, 3) == "vn ")
    {
      // glm::vec3
      glm::vec3 normal;
      sscanf_s(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
      m_Normals.push_back(normal);
    }
    else if (line.substr(0, 2) == "f ")
    {
      // Face
      IndexesFace face;
      unsigned int vertices[3];
      unsigned int tex_coords[3];
      unsigned int normals[3];
      // unsigned vt1, vt2, vt3, vn1, vn2, vn3;
      int matches = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertices[0], &tex_coords[0], &normals[0], &vertices[1], &tex_coords[1], &normals[1], &vertices[2], &tex_coords[2], &normals[2]);

      if (matches != 9)
      {
        // Normals are optional
        matches = sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &vertices[0], &tex_coords[0], &vertices[1], &tex_coords[1], &vertices[2], &tex_coords[2]);
        if (matches == 3)
        {
          // Texture coordinates and normals are optional
          matches = sscanf_s(line.c_str(), "f %d %d %d", &vertices[0], &vertices[1], &vertices[2]);
        }
        else
        {
          // Only vertices are specified
          sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &vertices[0], &normals[0], &vertices[1], &normals[1], &vertices[2], &normals[2]);
          tex_coords[0] = 0;
          tex_coords[1] = 0;
          tex_coords[2] = 0;
        }
      }
      vertices[0]--;
      vertices[1]--;
      vertices[2]--;
      if (tex_coords[0] != 0)
        tex_coords[0]--;
      if (tex_coords[1] != 0)
        tex_coords[1]--;
      if (tex_coords[2] != 0)
        tex_coords[2]--;
      normals[0]--;
      normals[1]--;
      normals[2]--;
      face.vertices[0] = vertices[0];
      face.vertices[1] = vertices[1];
      face.vertices[2] = vertices[2];
      face.texCoords[0] = tex_coords[0];
      face.texCoords[1] = tex_coords[1];
      face.texCoords[2] = tex_coords[2];
      face.normals[0] = normals[0];
      face.normals[1] = normals[1];
      face.normals[2] = normals[2];
      m_Faces.push_back(face);

      m_Face_id_to_material_map[face_index] = current_material;
      m_Face_to_material_map[face] = current_material;
      face_index++;
    }
    else if (line.substr(0, 7) == "usemtl ")
    {
      // Material
      current_material = line.substr(7);
    }
    else if (line.substr(0, 7) == "mtllib ")
    {
      // Material library
      std::string mtl_filename = line.substr(7);
      std::string mtl_filepath;

#if VISUAL_STUDIO
      mtl_filepath = "src/assets/obj/" + mtl_filename;
#elif MINGW
      mtl_filepath = "app\\src\\assets\\obj\\" + mtl_filename;
#endif

      std::ifstream mtl_file(mtl_filepath);
      if (mtl_file.is_open())
      {

        int i = 0;
        while (std::getline(mtl_file, line))
        {
          if (line.substr(0, 7) == "newmtl ")
          {
            if (i != 0)
            {
              m_Materials.push_back(material);
              m_Material_name_to_material_map[material.name] = material;
            }
            i++;
            // New material
            material = OBJMaterial();
            material.name = line.substr(7);
          }
          else if (line.substr(0, 3) == "Ka ")
          {
            // Ambient color
            sscanf_s(line.c_str(), "Ka %f %f %f", &material.ambient_color.r, &material.ambient_color.g, &material.ambient_color.b);
          }
          else if (line.substr(0, 3) == "Kd ")
          {
            // Diffuse color
            sscanf_s(line.c_str(), "Kd %f %f %f", &material.diffuse_color.r, &material.diffuse_color.g, &material.diffuse_color.b);
          }
          else if (line.substr(0, 3) == "Ks ")
          {
            // Specular color
            sscanf_s(line.c_str(), "Ks %f %f %f", &material.specular_color.r, &material.specular_color.g, &material.specular_color.b);
          }
          else if (line.substr(0, 3) == "Ke ")
          {
            // Emissive color
            sscanf_s(line.c_str(), "Ke %f %f %f", &material.emissive_color.r, &material.emissive_color.g, &material.emissive_color.b);
          }
          else if (line.substr(0, 2) == "Ns")
          {
            // Shininess
            sscanf_s(line.c_str(), "Ns %f", &material.shininess);
          }
          else if (line.substr(0, 2) == "Ni")
          {
            // Optical density
            sscanf_s(line.c_str(), "Ni %f", &material.optical_density);
          }
          else if (line.substr(0, 2) == "d ")
          {
            // Transparency
            sscanf_s(line.c_str(), "d %f", &material.transparency);
          }
          else if (line.substr(0, 5) == "illum")
          {
            // Transparency
            sscanf_s(line.c_str(), "illum %d", &material.illumination_model);
          }
          else if (line.substr(0, 6) == "map_Kd")
          {
            // Diffuse texture map
            material.diffuse_texture_map = line.substr(7);
          }
        }
        m_Materials.push_back(material);
      }
      else
      {
        std::cout << "Could not open file " << mtl_filename << std::endl;
      }
      mtl_file.close();
    }
  }
  obj_file.close();
}

std::vector<glm::vec3> OBJLoader::GetVertices()
{
  return m_Vertices;
}

std::vector<glm::vec2> OBJLoader::GetTextureCoordinates()
{
  return m_TexCoords;
}

std::vector<glm::vec3> OBJLoader::GetNormals()
{
  return m_Normals;
}

std::vector<IndexesFace> OBJLoader::GetFaces()
{
  return m_Faces;
}

std::vector<OBJMaterial> OBJLoader::GetMaterials()
{
  return m_Materials;
}

OBJLoader::~OBJLoader()
{
  // Clear the vectors
  m_Vertices.clear();
  m_TexCoords.clear();
  m_Normals.clear();
  m_Faces.clear();
  m_Materials.clear();
}

std::string OBJLoader::ToString()
{
  std::stringstream str;
  str << "Vertices:\n";
  for (const auto &v : m_Vertices)
  {
    str << "  " << v.x << ", " << v.y << ", " << v.z << "\n";
  }
  str << "Texture coordinates:\n";
  for (const auto &tc : m_TexCoords)
  {
    str << "  " << tc.x << ", " << tc.y << "\n";
  }
  str << "Normals:\n";
  for (const auto &n : m_Normals)
  {
    str << "  " << n.x << ", " << n.y << ", " << n.z << "\n";
  }
  str << "Faces:\n";
  for (const auto &f : m_Faces)
  {
    str << "  " << f.vertices[0] << ", " << f.vertices[1] << ", " << f.vertices[2] << ", " << f.texCoords[0] << ", " << f.texCoords[1] << ", " << f.texCoords[2] << ", " << f.normals[0] << ", " << f.normals[1] << ", " << f.normals[2] << "\n";
  }
  str << "Materials:\n";
  for (const auto &m : m_Materials)
  {
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

OBJMaterial OBJLoader::GetMaterialsFromFaceId(int face_id)
{
  // Look up the material name for the face index
  auto it = m_Face_id_to_material_map.find(face_id);
  if (it != m_Face_id_to_material_map.end())
  {
    std::string material_name = it->second;
    // Look up the full Material object for the material name
    auto it2 = m_Material_name_to_material_map.find(material_name);
    if (it2 != m_Material_name_to_material_map.end())
    {
      return it2->second;
    }
    else
    {
      // The material name was not found in the map
      return OBJMaterial();
    }
  }
  else
  {
    // The face index was not found in the map
    return OBJMaterial();
  }
}

OBJMaterial OBJLoader::GetMaterialsFromFace(IndexesFace &face)
{
  // Look up the material name for the face
  auto it = m_Face_to_material_map.find(face);
  if (it != m_Face_to_material_map.end())
  {
    std::string material_name = it->second;
    // Look up the full Material object for the material name
    auto it2 = m_Material_name_to_material_map.find(material_name);
    if (it2 != m_Material_name_to_material_map.end())
    {
      return it2->second;
    }
    else
    {
      // The material name was not found in the map
      return OBJMaterial();
    }
  }
  else
  {
    // The face was not found in the map
    return OBJMaterial();
  }
}
