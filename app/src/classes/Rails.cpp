#include "headers/Rails.h"

std::shared_ptr<Rails> Rails::Create(std::vector<glm::vec3> controlPoints)
{

    std::shared_ptr<Rails> rails = std::make_shared<Rails>(controlPoints);

    rails->m_Type = MeshType::RAILS;

    rails->m_Position = glm::vec3(0.0f);
    rails->m_Scale = glm::vec3(1.0f);
    rails->m_Rotation = glm::vec3(0.0f);
    rails->m_Matrix = glm::mat4(1.0f);

    for (int i = 0; i < controlPoints.size(); i++)
    {
        glm::vec3 point = controlPoints[i];
        std::shared_ptr<ControlPoint> controlPoint = std::make_shared<ControlPoint>(point, i);
        controlPoint->SetParent(rails);
        rails->m_Children.push_back(controlPoint);
    }

    rails->m_VBO_pos = nullptr;

    rails->Update();

    rails->CreateMaterial("basic");

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

Rails::Rails(std::vector<glm::vec3> controlPoints)
{
    // m_Type = MeshType::RAILS;

    // m_Position = glm::vec3(0.0f);
    // m_Scale = glm::vec3(1.0f);
    // m_Rotation = glm::vec3(0.0f);
    // m_Matrix = glm::mat4(1.0f);

    // for (int i = 0; i < controlPoints.size(); i++)
    // {
    //     glm::vec3 point = controlPoints[i];
    //     std::shared_ptr<ControlPoint> controlPoint = std::make_shared<ControlPoint>(point, i);

    //     // std::cout << "ControlPoint " << i << " created" << std::endl;
    //     // controlPoint->SetParent();
    //     // std::cout << "ControlPoint " << i << " created" << std::endl;
    //     m_Children.push_back(controlPoint);
    // }

    // m_VBO_pos = nullptr;

    // Update();

    // CreateMaterial("basic");

    // SetName("Rails");

    // m_VAO = new VertexArray();
    // m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    // VertexBufferLayout layout;
    // layout.Push<float>(3);
    // m_VAO->AddBuffer(*m_VBO_pos, layout);
    // m_VBO_pos->Unbind();
    // m_VAO->Unbind();
}

void Rails::Draw()
{

    m_VAO->Bind();
    m_Material->GetShader()->Bind();
    glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
    m_VAO->Unbind();
    m_Material->GetShader()->Unbind();

    for (auto point : m_Children)
    {
        point->Draw();
    }
}

void Rails::Update()
{
    m_Vertices.clear();

    for (int i = 4; i <= m_Children.size(); i += 3)
    {

        // ControlPoint *point1 = dynamic_cast<ControlPoint *>(m_Children[i - 4].get());
        // ControlPoint *point2 = dynamic_cast<ControlPoint *>(m_Children[i - 3].get());
        // ControlPoint *point3 = dynamic_cast<ControlPoint *>(m_Children[i - 2].get());
        // ControlPoint *point4 = dynamic_cast<ControlPoint *>(m_Children[i - 1].get());

        std::vector<glm::vec3> points({m_Children[i - 4].get()->GetVertices()[0],
                                       m_Children[i - 3].get()->GetVertices()[0],
                                       m_Children[i - 2].get()->GetVertices()[0],
                                       m_Children[i - 1].get()->GetVertices()[0]});

        BezierCurve curve(points);
        for (float t = 0; t <= 1.0; t += 0.1)
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