#include "headers/Rails.h"

std::shared_ptr<Rails> Rails::Create()
{
    std::shared_ptr<Rails> rails = std::make_shared<Rails>();

    rails->m_Type = MeshType::RAILS;

    rails->Update();

    rails->CreateMaterial("controlPoints");

    rails->SetName("Rails");

    VertexBufferLayout layout;
    layout.Push<float>(3);

    rails->m_VAO = new VertexArray();
    rails->m_VBO_pos = new VertexBuffer(rails->m_Vertices.data(), rails->m_Vertices.size() * sizeof(glm::vec3));
    rails->m_VAO->AddBuffer(*rails->m_VBO_pos, layout);
    rails->m_VBO_pos->Unbind();
    rails->m_VAO->Unbind();

    // tangent
    rails->m_VAO_tang = new VertexArray();
    rails->m_VBO_tang = new VertexBuffer(rails->m_VerticesTangents.data(), rails->m_VerticesTangents.size() * sizeof(glm::vec3));
    rails->m_VAO_tang->AddBuffer(*rails->m_VBO_tang, layout);
    rails->m_VBO_tang->Unbind();
    rails->m_VAO_tang->Unbind();

    rails->m_Material_tang = new Material();
    rails->m_Material_tang->AddShader("tangents");
    rails->m_Material_tang->GetShader()->Bind();
    rails->m_Material_tang->GetShader()->SetUniform3f("u_color", 1.0f, 1.0f, 0.0f);
    rails->m_Material_tang->GetShader()->Unbind();

    // final tangent
    rails->m_VAO_finalTang = new VertexArray();
    rails->m_VBO_finalTang = new VertexBuffer(rails->m_VerticesFinalTangents.data(), rails->m_VerticesFinalTangents.size() * sizeof(glm::vec3));
    rails->m_VAO_finalTang->AddBuffer(*rails->m_VBO_finalTang, layout);
    rails->m_VBO_finalTang->Unbind();
    rails->m_VAO_finalTang->Unbind();

    rails->m_Material_finalTang = new Material();
    rails->m_Material_finalTang->AddShader("tangents");
    rails->m_Material_finalTang->GetShader()->Bind();
    rails->m_Material_finalTang->GetShader()->SetUniform3f("u_color", .0f, 1.0f, 1.0f);
    rails->m_Material_finalTang->GetShader()->Unbind();

    // normal
    rails->m_VAO_norm = new VertexArray();
    rails->m_VBO_norm = new VertexBuffer(rails->m_VerticesNormals.data(), rails->m_VerticesNormals.size() * sizeof(glm::vec3));
    rails->m_VAO_norm->AddBuffer(*rails->m_VBO_norm, layout);
    rails->m_VBO_norm->Unbind();
    rails->m_VAO_norm->Unbind();

    rails->m_Material_norm = new Material();
    rails->m_Material_norm->AddShader("tangents");
    rails->m_Material_norm->GetShader()->Bind();
    rails->m_Material_norm->GetShader()->SetUniform3f("u_color", .0f, .0f, 1.0f);
    rails->m_Material_norm->GetShader()->Unbind();

    // binormal
    rails->m_VAO_binormal = new VertexArray();
    rails->m_VBO_binormal = new VertexBuffer(rails->m_VerticesBinormals.data(), rails->m_VerticesBinormals.size() * sizeof(glm::vec3));
    rails->m_VAO_binormal->AddBuffer(*rails->m_VBO_binormal, layout);
    rails->m_VBO_binormal->Unbind();
    rails->m_VAO_binormal->Unbind();

    rails->m_Material_binormal = new Material();
    rails->m_Material_binormal->AddShader("tangents");
    rails->m_Material_binormal->GetShader()->Bind();
    rails->m_Material_binormal->GetShader()->SetUniform3f("u_color", .0f, 1.0f, .0f);
    rails->m_Material_binormal->GetShader()->Unbind();

    return rails;
}

Rails::Rails()
{
    m_VAO = nullptr;
    m_VBO_pos = nullptr;
    m_VBO_norm = nullptr;
    m_IBO = nullptr;
    m_Material = nullptr;
    m_Parent = nullptr;
    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    LoadControlPointsFiles();
}

void Rails::RemoveChildren(std::shared_ptr<Mesh> child)
{
    Mesh::RemoveChildren(child);

    for (int i = 0; i < m_Children.size(); i++)
    {
        m_Children[i].get()->SetName("ControlPoint_" + std::to_string(i));
    }
}

void Rails::Draw()
{

    if (!m_DrawRails && m_Vertices.size() > 0)
    {
        m_VAO->Bind();
        m_Material->GetShader()->Bind();
        glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
        m_VAO->Unbind();
        m_Material->GetShader()->Unbind();

        // m_VAO_tang->Bind();
        // m_Material_tang->GetShader()->Bind();
        // glDrawArrays(GL_LINES, 0, m_VerticesTangents.size());
        // m_VAO_tang->Unbind();
        // m_Material_tang->GetShader()->Unbind();

        m_VAO_finalTang->Bind();
        m_Material_finalTang->GetShader()->Bind();
        glDrawArrays(GL_LINES, 0, m_VerticesFinalTangents.size());
        m_VAO_finalTang->Unbind();
        m_Material_finalTang->GetShader()->Unbind();

        m_VAO_norm->Bind();
        m_Material_norm->GetShader()->Bind();
        glDrawArrays(GL_LINES, 0, m_VerticesNormals.size());
        m_VAO_norm->Unbind();
        m_Material_norm->GetShader()->Unbind();

        m_VAO_binormal->Bind();
        m_Material_binormal->GetShader()->Bind();
        glDrawArrays(GL_LINES, 0, m_VerticesBinormals.size());
        m_VAO_binormal->Unbind();
        m_Material_binormal->GetShader()->Unbind();

        for (auto child : m_Children)
            child->Draw();

        return;
    }

    if (m_Rails.size() == 0)
        return;

    for (auto rail : m_Rails)
        rail->Draw();
}

void Rails::Update()
{
    m_Vertices.clear();
    m_VerticesTangents.clear();
    m_VerticesFinalTangents.clear();
    m_VerticesNormals.clear();
    m_VerticesBinormals.clear();

    for (int i = 4; i <= m_Children.size(); i += 3)
    {

        std::vector<glm::vec3> points({m_Children[i - 4].get()->GetVertices()[0],
                                       m_Children[i - 3].get()->GetVertices()[0],
                                       m_Children[i - 2].get()->GetVertices()[0],
                                       m_Children[i - 1].get()->GetVertices()[0]});

        BezierCurve curve(points);
        for (float t = 0; t <= 1; t += 0.01)
        {
            glm::vec3 curvePoint = curve.GetPoint(t);
            glm::vec3 vertex(curvePoint.x, curvePoint.y, curvePoint.z);
            m_Vertices.push_back(vertex);

            glm::vec3 tangent = curve.GetTangent(t);
            tangent = glm::normalize(tangent);
            m_VerticesTangents.push_back(vertex);
            m_VerticesTangents.push_back(vertex + tangent);

            glm::vec3 normalisedTangent = curve.GetNormalisedTangent(t, curvePoint, tangent);
            normalisedTangent *= 10.0f;

            m_VerticesFinalTangents.push_back(vertex);
            m_VerticesFinalTangents.push_back(vertex + normalisedTangent);

            glm::vec3 normal = glm::cross(normalisedTangent, tangent);
            m_VerticesNormals.push_back(vertex);
            m_VerticesNormals.push_back(vertex + normal);

            glm::vec3 biNormal = glm::cross(normalisedTangent, normal);
            m_VerticesBinormals.push_back(vertex);
            m_VerticesBinormals.push_back(vertex + biNormal);
        }
    }

    if (m_VBO_pos == nullptr)
        return;

    m_VBO_pos->Bind();
    m_VBO_pos->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    m_VBO_pos->Unbind();

    m_VBO_tang->Bind();
    m_VBO_tang->SetData(m_VerticesTangents.data(), m_VerticesTangents.size() * sizeof(glm::vec3));
    m_VBO_tang->Unbind();

    m_VBO_finalTang->Bind();
    m_VBO_finalTang->SetData(m_VerticesFinalTangents.data(), m_VerticesFinalTangents.size() * sizeof(glm::vec3));
    m_VBO_finalTang->Unbind();

    m_VBO_norm->Bind();
    m_VBO_norm->SetData(m_VerticesNormals.data(), m_VerticesNormals.size() * sizeof(glm::vec3));
    m_VBO_norm->Unbind();

    m_VBO_binormal->Bind();
    m_VBO_binormal->SetData(m_VerticesBinormals.data(), m_VerticesBinormals.size() * sizeof(glm::vec3));
    m_VBO_binormal->Unbind();

    if (m_Children.size() == 0)
        return;

    for (auto point : m_Children)
    {
        point->GetVBO()->Bind();
        point->GetVBO()->SetData(point->GetVertices().data(), point->GetVertices().size() * sizeof(glm::vec3));
        point->GetVBO()->Unbind();
    }
}

void Rails::UpdateRails()
{
    m_Rails.clear();

    glm::vec3 prevPosition = m_Vertices[0];
    std::shared_ptr<Rail> rail = std::make_shared<Rail>("rail.obj", 0);
    rail->SetPosition(prevPosition);
    m_Rails.push_back(rail);

    int railIndex = 1;
    for (int i = 1; i < m_Vertices.size(); i++)
    {
        glm::vec3 currentPosition = m_Vertices[i];
        glm::vec3 direction = currentPosition - prevPosition;
        float length = glm::length(direction);

        if (glm::length(length) > RAIL_WIDTH)
        {
            std::shared_ptr<Rail> rail = std::make_shared<Rail>("rail.obj", railIndex++);
            rail->SetPosition(currentPosition);
            m_Rails.push_back(rail);
            prevPosition = currentPosition;

            // compute rotation
            glm::vec3 tangent = m_VerticesFinalTangents[i];
            glm::vec3 normal = m_VerticesNormals[i];
            glm::vec3 binormal = m_VerticesBinormals[i];

            // compute axis and angle
            glm::vec3 axis = glm::cross(tangent, normal);
            float angle = glm::acos(glm::dot(tangent, normal));

            rail->Rotate(angle, axis);
        }
    }
}

void Rails::GenerateControlPoints(std::vector<glm::vec3> controlPoints)
{
    m_Children.clear();
    for (int i = 0; i < controlPoints.size(); i++)
    {
        glm::vec3 point = controlPoints[i];
        std::shared_ptr<ControlPoint> controlPoint = std::make_shared<ControlPoint>(point, i);
        AddChildren(controlPoint);
    }
}

void Rails::LoadRails(const std::string filename)
{
    std::string filepath;

#if VISUAL_STUDIO
    filepath = "src/assets/rails/";
#elif MINGW
    filepath = "app\\src\\assets\\rails\\";
#endif

    std::ifstream file(filepath + filename);
    std::string line;

    std::vector<glm::vec3> controlPoints;

    while (std::getline(file, line))
    {
        glm::vec3 vertex;
        sscanf_s(line.c_str(), "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
        glm::vec3 point(vertex.x, vertex.y, vertex.z);
        controlPoints.push_back(point);
    }

    GenerateControlPoints(controlPoints);
    Update();
}

bool Rails::ExportRails()
{

    std::string filepath;

#if VISUAL_STUDIO
    filepath = "src/assets/rails/";
#elif MINGW
    filepath = "app\\src\\assets\\rails\\";
#endif

    std::string filename = m_ControlPointsFileName + ".txt";

    bool exists = true;
    int i = 0;
    struct stat buf;
    while (exists)
    {
        std::string file = filepath + filename;
        if (stat(file.c_str(), &buf) != -1)
        {
            i++;
            filename = m_ControlPointsFileName + "_" + std::to_string(i) + ".txt";
        }
        else
            exists = false;
    }

    m_ControlPointsFileName = filepath + filename;

    std::ofstream file(m_ControlPointsFileName);

    if (!file.is_open())
        return false;

    for (auto point : m_Children)
    {
        glm::vec3 vertex = point.get()->GetVertices()[0];
        file << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    file.close();
    return true;
}

void Rails::LoadControlPointsFiles()
{
    m_ControlPointsFiles.clear();

    std::string filepath;

#if VISUAL_STUDIO
    filepath = "src/assets/rails/";
#elif MINGW
    filepath = "app\\src\\assets\\rails\\";
#endif

    // load all files in directory
    for (const auto &entry : std::filesystem::directory_iterator(filepath))
    {
        std::string filename = entry.path().filename().string();
        m_ControlPointsFiles.push_back(filename);
    }
}

bool Rails::DeleteFileRails(const std::string filename)
{
    std::string filepath;

#if VISUAL_STUDIO
    filepath = "src/assets/rails/";
#elif MINGW
    filepath = "app\\src\\assets\\rails\\";
#endif

    std::string file = filepath + filename;

    if (remove(file.c_str()) != 0)
        return false;
    else
        return true;
}