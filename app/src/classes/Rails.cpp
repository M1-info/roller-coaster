#include "headers/Rails.h"

Rails::Rails() : m_VAO_lines(nullptr),m_VAO_points(nullptr), m_VBO_lines(nullptr), m_VBO_points(nullptr)
{
}

Rails::Rails(std::vector<glm::vec3> controlPoints) : m_VAO_lines(nullptr),m_VAO_points(nullptr), m_VBO_lines(nullptr), m_VBO_points(nullptr)
{
    CreateFromControlPoints(controlPoints);
}

Rails::~Rails()
{
}

void Rails::CreateFromControlPoints(std::vector<glm::vec3> controlPoints)
{

    m_ControlPoints = controlPoints;

    for(int i = 4; i <= m_ControlPoints.size(); i+=3){
        std::vector<glm::vec3> points({m_ControlPoints[i - 4], m_ControlPoints[i - 3], m_ControlPoints[i - 2], m_ControlPoints[i - 1]});
        BezierCurve curve(points);
        for(float t = 0; t <= 1.0; t += 0.1){
            glm::vec3 curvePoints = curve.GetPoint(t);
            Vertex vertex(curvePoints.x, curvePoints.y, curvePoints.z);
            m_Vertices.push_back(vertex);
        }
    }

    m_Shader = new Shader("basic");

    m_VAO_lines = new VertexArray();
    m_VBO_lines = new VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    VertexBufferLayout layout_lines;
    layout_lines.Push<float>(3);
    m_VAO_lines->AddBuffer(*m_VBO_lines, layout_lines);
    m_VBO_lines->Unbind();
    m_VAO_lines->Unbind();

    m_VAO_points = new VertexArray();
    m_VBO_points = new VertexBuffer(m_ControlPoints.data(), m_ControlPoints.size() * sizeof(glm::vec3));
    VertexBufferLayout layout_points;
    layout_points.Push<float>(3);
    m_VAO_points->AddBuffer(*m_VBO_points, layout_points);
    m_VBO_points->Unbind();
    m_VAO_points->Unbind();

}

void Rails::Draw()
{
    m_VAO_lines->Bind();
    m_Shader->Bind();
    glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size());
    m_VAO_lines->Unbind();
    m_Shader->Unbind();

    // draw control points
    glEnable(GL_PROGRAM_POINT_SIZE);
    m_VAO_points->Bind();
    m_Shader->Bind();
    glDrawArrays(GL_POINTS, 0, m_ControlPoints.size());
    m_VAO_points->Unbind();
    m_Shader->Unbind();
    glDisable(GL_PROGRAM_POINT_SIZE);
    
}