#include "meshes/Rails.h"

std::shared_ptr<Rails> Rails::Create()
{
    std::shared_ptr<Rails> rails = std::make_shared<Rails>();

    rails->Update();

    rails->CreateMaterial("controlPoints");

    rails->SetName("Rails");

    VertexBufferLayout layout;
    layout.Push<float>(3);

    rails->m_Material_curve = new Material();
    rails->m_Material_curve->AddShader("tangents");

    rails->m_VAO = new VertexArray();
    rails->m_VBO_positions = new VertexBuffer(rails->m_Vertices.data(), rails->m_Vertices.size() * sizeof(glm::vec3));
    rails->m_VAO->AddBuffer(*rails->m_VBO_positions, layout);
    rails->m_VBO_positions->Unbind();
    rails->m_VAO->Unbind();

    // tangent
    rails->m_VAO_tangents = new VertexArray();
    rails->m_VBO_tangents = new VertexBuffer(rails->m_Tangents.data(), rails->m_Tangents.size() * sizeof(glm::vec3));
    rails->m_VAO_tangents->AddBuffer(*rails->m_VBO_tangents, layout);
    rails->m_VBO_tangents->Unbind();
    rails->m_VAO_tangents->Unbind();

    // // normal
    rails->m_VAO_normals = new VertexArray();
    rails->m_VBO_normals = new VertexBuffer(rails->m_Normals.data(), rails->m_Normals.size() * sizeof(glm::vec3));
    rails->m_VAO_normals->AddBuffer(*rails->m_VBO_normals, layout);
    rails->m_VBO_normals->Unbind();
    rails->m_VAO_normals->Unbind();

    // // binormal
    rails->m_VAO_binormals = new VertexArray();
    rails->m_VBO_binormals = new VertexBuffer(rails->m_Binormals.data(), rails->m_Binormals.size() * sizeof(glm::vec3));
    rails->m_VAO_binormals->AddBuffer(*rails->m_VBO_binormals, layout);
    rails->m_VBO_binormals->Unbind();
    rails->m_VAO_binormals->Unbind();

    return rails;
}

Rails::Rails()
{
    m_VAO = nullptr;
    m_VBO_positions = nullptr;
    m_VBO_normals = nullptr;
    m_IBO = nullptr;
    m_Material = nullptr;
    m_Parent = nullptr;
    m_Type = MeshType::RAILS;
    m_DrawRails = false;
    m_Transform = new Transform();
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
        m_VBO_positions->Bind();
        m_Material->GetShader()->Bind();
        glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
        m_VAO->Unbind();
        m_VBO_positions->Unbind();
        m_Material->GetShader()->Unbind();

        m_VAO_tangents->Bind();
        m_Material_curve->GetShader()->Bind();
        m_Material_curve->GetShader()->SetUniform3f("u_color", 1.0f, 1.0f, 0.0f);
        glDrawArrays(GL_LINES, 0, m_Tangents.size());
        m_VAO_tangents->Unbind();
        m_Material_curve->GetShader()->Unbind();

        m_VAO_normals->Bind();
        m_Material_curve->GetShader()->Bind();
        m_Material_curve->GetShader()->SetUniform3f("u_color", 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 0, m_Normals.size());
        m_VAO_normals->Unbind();
        m_Material_curve->GetShader()->Unbind();

        m_VAO_binormals->Bind();
        m_Material_curve->GetShader()->Bind();
        m_Material_curve->GetShader()->SetUniform3f("u_color", 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_LINES, 0, m_Binormals.size());
        m_VAO_binormals->Unbind();
        m_Material_curve->GetShader()->Bind();

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
    m_Tangents.clear();
    m_Normals.clear();
    m_Binormals.clear();

    for (int i = 4; i <= m_Children.size(); i += 3)
    {

        std::vector<glm::vec3> points({m_Children[i - 4].get()->GetVertices()[0],
                                       m_Children[i - 3].get()->GetVertices()[0],
                                       m_Children[i - 2].get()->GetVertices()[0],
                                       m_Children[i - 1].get()->GetVertices()[0]});

        CatmullRom curve(points);
        for (float t = 0; t <= 1; t += 0.0001)
        {
            glm::vec3 currentPoint = curve.GetPoint(t);
            glm::vec3 nextPoint = curve.GetPoint(t + 0.0001);
            glm::vec3 vertex(currentPoint.x, currentPoint.y, currentPoint.z);
            m_Vertices.push_back(vertex);

            // compute TNB
            glm::vec3 T = glm::normalize(nextPoint - currentPoint);
            glm::vec3 B = glm::normalize(glm::cross(T, nextPoint + currentPoint));
            glm::vec3 N = glm::normalize(glm::cross(B, T));

            m_Tangents.push_back(T);
            m_Normals.push_back(N);
            m_Binormals.push_back(B);
        }
    }

    if (m_VBO_positions == nullptr)
        return;

    m_VBO_positions->Bind();
    m_VBO_positions->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    m_VBO_positions->Unbind();

    m_VBO_tangents->Bind();
    m_VBO_tangents->SetData(m_Tangents.data(), m_Tangents.size() * sizeof(glm::vec3));
    m_VBO_tangents->Unbind();

    m_VBO_normals->Bind();
    m_VBO_normals->SetData(m_Normals.data(), m_Normals.size() * sizeof(glm::vec3));
    m_VBO_normals->Unbind();

    m_VBO_binormals->Bind();
    m_VBO_binormals->SetData(m_Binormals.data(), m_Binormals.size() * sizeof(glm::vec3));
    m_VBO_binormals->Unbind();

    if (m_Children.size() == 0)
        return;

    for (auto point : m_Children)
    {
        point->GetVBOPositions()->Bind();
        point->GetVBOPositions()->SetData(point->GetVertices().data(), point->GetVertices().size() * sizeof(glm::vec3));
        point->GetVBOPositions()->Unbind();
    }
}

void Rails::UpdateRails()
{
    m_Rails.clear();

    glm::vec3 prevPosition = m_Vertices[0];
    std::shared_ptr<Rail> rail = std::make_shared<Rail>("rail.obj", 0);
    rail->GetTransform()->SetPosition(prevPosition);
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
            rail->GetTransform()->SetPosition(currentPosition);
            m_Rails.push_back(rail);
            prevPosition = currentPosition;

            // compute rotation
            glm::vec3 tangent = m_Tangents[i];
            glm::vec3 normal = m_Normals[i];
            glm::vec3 binormal = m_Binormals[i];

            // use frene frame to compute rotation
            glm::mat4 rotation = glm::mat4(1.0f);
            rotation[0] = glm::vec4(tangent, 0.0f);
            rotation[1] = glm::vec4(normal, 0.0f);
            rotation[2] = glm::vec4(binormal, 0.0f);
            rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            // convert to quaternion
            glm::quat rotationQuat = glm::quat_cast(rotation);

            // convert to euler angles
            glm::vec3 rotationEuler = glm::eulerAngles(rotationQuat);

            // set rotation
            rail->GetTransform()->SetRotation(rotationEuler);
            rail->GetTransform()->SetIsDirty(true);
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
