#include "headers/Rails.h"

std::shared_ptr<Rails> Rails::Create()
{
    std::shared_ptr<Rails> rails = std::make_shared<Rails>();

    rails->m_Type = MeshType::RAILS;

    rails->Update();

    rails->CreateMaterial("controlPoints");

    rails->SetName("Rails");

    rails->m_VAO = new VertexArray();
    rails->m_VBO_pos = new VertexBuffer(rails->m_Vertices.data(), rails->m_Vertices.size() * sizeof(glm::vec3));

    VertexBufferLayout layout;
    layout.Push<float>(3);

    rails->m_VAO->AddBuffer(*rails->m_VBO_pos, layout);
    rails->m_VBO_pos->Unbind();
    rails->m_VAO->Unbind();

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

    if (!m_DrawRails)
    {
        m_VAO->Bind();
        m_Material->GetShader()->Bind();
        glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
        m_VAO->Unbind();
        m_Material->GetShader()->Unbind();

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

    for (int i = 4; i <= m_Children.size(); i += 3)
    {

        std::vector<glm::vec3> points({m_Children[i - 4].get()->GetVertices()[0],
                                       m_Children[i - 3].get()->GetVertices()[0],
                                       m_Children[i - 2].get()->GetVertices()[0],
                                       m_Children[i - 1].get()->GetVertices()[0]});

        BezierCurve curve(points);
        for (float t = 0; t <= 1.0; t += 0.01)
        {
            glm::vec3 curvePoints = curve.GetPoint(t);
            glm::vec3 vertex(curvePoints.x, curvePoints.y, curvePoints.z);
            m_Vertices.push_back(vertex);
        }
    }

    if (m_VBO_pos == nullptr)
        return;

    m_VBO_pos->Bind();
    m_VBO_pos->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    m_VBO_pos->Unbind();

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

    for (int i = 0; i < m_Vertices.size(); i += 5)
    {
        std::shared_ptr<Rail> rail = std::make_shared<Rail>("rail.obj");
        rail->m_Position = m_Vertices[i];
        m_Rails.push_back(rail);
    }

    for (int i = 0; i < m_Rails.size(); i++)
    {
        if (i == m_Rails.size() - 1)
        {
            m_Rails[i].get()->m_Rotation.z = m_Rails[i - 1].get()->m_Rotation.z;
            continue;
        }

        glm::vec3 rail1 = m_Rails[i].get()->m_Position;
        glm::vec3 rail2 = m_Rails[i + 1].get()->m_Position;

        glm::vec3 direction = rail2 - rail1;
        direction = glm::normalize(direction);

        float angle = atan2(direction.y, direction.x);

        m_Rails[i].get()->m_Rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

        // Store the rotation of the preceding rail in a temporary vector
        // glm::vec3 prevRotation;
        // if (i == 0)
        //     prevRotation = glm::vec3(0.0f);
        // else
        //     prevRotation = m_Rails[i - 1].get()->m_Rotation;

        // // Calculate the direction between the current and next rail, and rotate it by the same angles as the preceding rail
        // glm::vec3 direction = rail2 - rail1;
        // direction = glm::rotateZ(direction, prevRotation.z);
        // direction = glm::rotateY(direction, prevRotation.y);
        // direction = glm::rotateX(direction, prevRotation.x);

        // // Calculate the angles for the current rail's rotation using the rotated direction vector
        // float angleX = acos(direction.x);
        // float angleY = -asin(direction.y);
        // float angleZ = atan(direction.z);

        // m_Rails[i].get()->m_Rotation.x = angleX;
        // m_Rails[i].get()->m_Rotation.y = angleY;
        // m_Rails[i].get()->m_Rotation.z = angleZ;
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
    UpdateRails();
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