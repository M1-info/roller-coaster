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

  std::cout << "Loading " << obj_filename << "..." << std::endl;

  while (std::getline(obj_file, line))
  {
    if (line.substr(0, 2) == "v ")
    {
      // Vertex
      OBJVertex vertex;
      sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      m_Vertices.push_back(vertex);
    }
    else if (line.substr(0, 3) == "vt ")
    {
      // Texture coordinate
      OBJTextureCoordinate tex_coord;
      sscanf(line.c_str(), "vt %f %f", &tex_coord.u, &tex_coord.v);
      m_TexCoords.push_back(tex_coord);
    }
    else if (line.substr(0, 3) == "vn ")
    {
      // Normal
      OBJNormal normal;
      sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
      m_Normals.push_back(normal);
    }
    else if (line.substr(0, 2) == "f ")
    {
      // Face
      OBJFace face;
      int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &face.v1, &face.vt1, &face.vn1, &face.v2, &face.vt2, &face.vn2, &face.v3, &face.vt3, &face.vn3);
      if (matches != 9)
      {
        // Normals are optional
        matches = sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &face.v1, &face.vt1, &face.v2, &face.vt2, &face.v3, &face.vt3);
        if (matches != 6)
        {
          // Texture coordinates and normals are optional
          matches = sscanf(line.c_str(), "f %d %d %d", &face.v1, &face.v2, &face.v3);
        }
        if (matches == 3)
        {
          // Only vertices are specified
          sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &face.v1, &face.vn1, &face.v2, &face.vn2, &face.v3, &face.vn3);
          face.vt1 = 0;
          face.vt2 = 0;
          face.vt3 = 0;
        }
      }
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
        std::cout << "Loading " << mtl_filename << "..." << std::endl;
        int i = 0;
        while (std::getline(mtl_file, line))
        {
          if (line.substr(0, 7) == "newmtl ")
          {
            std::cout << "Loading " << line << "..." << std::endl;
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
            sscanf(line.c_str(), "Ka %f %f %f", &material.ambient_color.r, &material.ambient_color.g, &material.ambient_color.b);
          }
          else if (line.substr(0, 3) == "Kd ")
          {
            // Diffuse color
            sscanf(line.c_str(), "Kd %f %f %f", &material.diffuse_color.r, &material.diffuse_color.g, &material.diffuse_color.b);
          }
          else if (line.substr(0, 3) == "Ks ")
          {
            // Specular color
            sscanf(line.c_str(), "Ks %f %f %f", &material.specular_color.r, &material.specular_color.g, &material.specular_color.b);
          }
          else if (line.substr(0, 3) == "Ke ")
          {
            // Emissive color
            sscanf(line.c_str(), "Ke %f %f %f", &material.emissive_color.r, &material.emissive_color.g, &material.emissive_color.b);
          }
          else if (line.substr(0, 2) == "Ns")
          {
            // Shininess
            sscanf(line.c_str(), "Ns %f", &material.shininess);
          }
          else if (line.substr(0, 2) == "Ni")
          {
            // Optical density
            sscanf(line.c_str(), "Ni %f", &material.optical_density);
          }
          else if (line.substr(0, 2) == "d ")
          {
            // Transparency
            sscanf(line.c_str(), "d %f", &material.transparency);
          }
          else if (line.substr(0, 5) == "illum")
          {
            // Transparency
            sscanf(line.c_str(), "illum %d", &material.illumination_model);
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

std::vector<OBJVertex> OBJLoader::GetVertices()
{
  return m_Vertices;
}

std::vector<OBJTextureCoordinate> OBJLoader::GetTextureCoordinates()
{
  return m_TexCoords;
}

std::vector<OBJNormal> OBJLoader::GetNormals()
{
  return m_Normals;
}

std::vector<OBJFace> OBJLoader::GetFaces()
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
    str << "  " << tc.u << ", " << tc.v << "\n";
  }
  str << "Normals:\n";
  for (const auto &n : m_Normals)
  {
    str << "  " << n.x << ", " << n.y << ", " << n.z << "\n";
  }
  str << "Faces:\n";
  for (const auto &f : m_Faces)
  {
    str << "  " << f.v1 << ", " << f.v2 << ", " << f.v3 << ", " << f.vt1 << ", " << f.vt2 << ", " << f.vt3 << ", " << f.vn1 << ", " << f.vn2 << ", " << f.vn3 << "\n";
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

OBJMaterial OBJLoader::GetMaterialsFromFace(OBJFace &face)
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
