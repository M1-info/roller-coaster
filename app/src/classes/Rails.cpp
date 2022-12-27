#include "headers/Rails.h"

Rails::Rails(std::vector<glm::vec3> controlPoints)
{

    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    for(auto point : controlPoints){
        std::shared_ptr<ControlPoint> controlPoint = std::make_shared<ControlPoint>(point);
        controlPoint->SetParent((std::shared_ptr<Mesh>)this);
        m_Children.push_back(controlPoint);
    }

    for(int i = 4; i <= m_Children.size(); i+=3){

        ControlPoint * point1 = dynamic_cast<ControlPoint*>(m_Children[i - 4].get());
        ControlPoint * point2 = dynamic_cast<ControlPoint*>(m_Children[i - 3].get());
        ControlPoint * point3 = dynamic_cast<ControlPoint*>(m_Children[i - 2].get());
        ControlPoint * point4 = dynamic_cast<ControlPoint*>(m_Children[i - 1].get());

        std::vector<glm::vec3> points({
            point1->GetVertices(), 
            point2->GetVertices(), 
            point3->GetVertices(), 
            point4->GetVertices()
        });

        BezierCurve curve(points);
        for(float t = 0; t <= 1.0; t += 0.1){
            glm::vec3 curvePoints = curve.GetPoint(t);
            Vertex vertex(curvePoints.x, curvePoints.y, curvePoints.z);
            m_Vertices.push_back(vertex);
        }
    }

    CreateMaterial("basic");

    SetName("Rails");

    m_VAO = new VertexArray();
    m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO_pos, layout);
    m_VBO_pos->Unbind();
    m_VAO->Unbind();
}

void Rails::Draw()
{

    m_VAO->Bind();
    m_Material->GetShader()->Bind();
    glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
    m_VAO->Unbind();
    m_Material->GetShader()->Unbind();

    for(auto point : m_Children){
        point->Draw();
    }
}