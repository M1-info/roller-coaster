#include "meshes/Rails.h"

std::shared_ptr<Rails> Rails::Create()
{
    std::shared_ptr<Rails> rails = std::make_shared<Rails>();

    rails->m_Type = MeshType::RAILS;
    rails->m_Transform = new Transform();
    rails->m_DrawRails = false;

    rails->CreateMaterial("controlPoints");

    rails->m_Name = "Rails";

    VertexBufferLayout layout;
    layout.Push<float>(3);

    rails->m_VAO = new VertexArray();
    rails->m_VBO_positions = new VertexBuffer(rails->m_Vertices.data(), rails->m_Vertices.size() * sizeof(glm::vec3));
    rails->m_VAO->AddBuffer(*rails->m_VBO_positions, layout);
    rails->m_VBO_positions->Unbind();
    rails->m_VAO->Unbind();

    return rails;
}

Rails::Rails()
{
    m_VBO_normals = nullptr;
    m_IBO = nullptr;
    m_Parent = nullptr;
}

void Rails::RemoveChildren(std::shared_ptr<Mesh> child)
{
    // Remove the child from the list of children
    Mesh::RemoveChildren(child);

    // Update the control points to rename them correctly
    for (int i = 0; i < m_Children.size(); i++)
    {
        m_Children[i].get()->SetName("ControlPoint_" + std::to_string(i));
    }
}

void Rails::Draw()
{

    m_VAO->Bind();
    m_VBO_positions->Bind();
    m_Material->GetShader()->Bind();
    glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
    m_VAO->Unbind();
    m_VBO_positions->Unbind();
    m_Material->GetShader()->Unbind();

    // Draw the control points
    for (auto child : m_Children)
        child->Draw();

    // If we don't want to draw the rails, return
    if (!m_DrawRails || m_Rails.size() == 0)
        return;

    // Draw the rails
    for (auto rail : m_Rails)
        rail->Draw();
}

void Rails::Update()
{
    UpdateControlPoints();

    if (m_DrawRails)
        UpdateRails();

    m_Transform->SetIsDirty(true);

    for (auto rail : m_Rails)
        rail->GetTransform()->SetIsDirty(true);

    for (auto child : m_Children)
        child->GetTransform()->SetIsDirty(true);
}

void Rails::UpdateControlPoints()
{

    m_Vertices.clear();
    m_Tangents.clear();
    m_Curves.clear();

    // step is 3 for bezier and 1 for bspline and catmull rom
    int step = 3;

    if (m_CurveType == CurveType::BSPLINE || m_CurveType == CurveType::CATMULL_ROM)
        step = 1;

    for (int i = 4; i <= m_Children.size(); i += step)
    {

        // Get the points for the curve
        std::vector<glm::vec3> points({m_Children[i - 4].get()->GetVertices()[0],
                                       m_Children[i - 3].get()->GetVertices()[0],
                                       m_Children[i - 2].get()->GetVertices()[0],
                                       m_Children[i - 1].get()->GetVertices()[0]});

        Curve curve;
        if (m_CurveType == CurveType::BEZIER)
            curve = BezierCubic(points);
        else if (m_CurveType == CurveType::CATMULL_ROM)
            curve = CatmullRom(points);
        else if (m_CurveType == CurveType::BSPLINE)
            curve = Bspline(points);

        m_Curves.push_back(curve);

        // Get the points for the curve
        for (float t = 0; t <= 1; t += 0.01)
        {
            glm::vec3 currentPoint = curve.GetPoint(t);
            glm::vec3 vertex(currentPoint.x, currentPoint.y, currentPoint.z);
            m_Vertices.push_back(vertex);

            glm::vec3 tangent = curve.GetTangent(t, currentPoint);
            m_Tangents.push_back(tangent);
        }
    }

    if (m_VBO_positions == nullptr)
        return;

    // Update the VBO for the curve
    m_VBO_positions->Bind();
    m_VBO_positions->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec3));
    m_VBO_positions->Unbind();

    if (m_Children.size() == 0)
        return;

    // update the VBO for the control points
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

    // Create the rails

    glm::vec3 prevPosition = glm::vec3(0.0f);
    int index = 0;
    for (int i = 0; i < m_Vertices.size(); i++)
    {
        // Get the current position
        glm::vec3 currentPosition = m_Vertices[i];
        glm::vec3 direction = currentPosition - prevPosition;
        float length = glm::length(direction);

        // If the length is greater than the rail width, create a new rail
        if (glm::length(length) > RAIL_WIDTH)
        {
            std::shared_ptr<Rail> rail = Rail::Create(index);
            rail->GetTransform()->SetPosition(currentPosition);

            m_Rails.push_back(rail);
            rail->SetParent(shared_from_this());
            rail->GetTransform()->SetParentTransform(m_Transform);
            prevPosition = currentPosition;

            // get the tangent
            glm::vec3 tangent = m_Tangents[i];

            // use the tangent to calculate the rotation
            float pitch = 0.0f;
            float yaw = glm::degrees(std::atan2(tangent.x, tangent.z)) + 90.0f;
            float roll = glm::degrees(std::atan2(direction.y, direction.x));

            // correct the yaw if the direction is negative
            if (direction.z < 0 && direction.x < 0)
                yaw *= -1;

            rail->GetTransform()->SetRotation(glm::vec3(pitch, yaw, roll));
            rail->GetTransform()->SetIsDirty(true);

            // rail->GeneratePlots(index);

            // update all children
            for (auto child : rail->GetChildren())
                child->GetTransform()->SetIsDirty(true);

            index++;
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
