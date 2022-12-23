#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Mesh.h"
#include "classes/headers/Scene.h"

int main(void)
{
    Renderer* renderer = new Renderer();
    renderer->Init();

    std::vector<float> vertices = {
        -0.50, -0.50,  0.50,
        -0.50,  0.50,  0.50,
         0.50,  0.50,  0.50,
         0.50, -0.50,  0.50,
        -0.50, -0.50, -0.50,
        -0.50,  0.50, -0.50,
         0.50,  0.50, -0.50,
         0.50, -0.50, -0.50,
    };

    std::vector<unsigned int> indices = {
        0, 2, 1, 
        0, 3, 2, // front
        4, 5, 6, 
        4, 6, 7, // back
        0, 1, 4, 
        4, 1, 5, // left
        2, 3, 7, 
        2, 7, 6, // right
        1, 2, 6,
        1, 6, 7, // top
        0, 7, 3, 
        0, 4, 7, // bottom
    };

    Mesh *mesh = new Mesh("Cube yellow", vertices, indices);
    mesh->AddShader("basic");
    mesh->Translate(glm::vec3(1, 0, 0));
    mesh->SetUp();
    mesh->GetShader()->SetUniform4f("u_color", 1.0, 1.0, 0.0, 1.0);
    mesh->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh->GetShader()->SetUniformMat4f("u_model", mesh->GetMatrix());
    mesh->Clear();

    Mesh *mesh2 = new Mesh("Cube vert", vertices, indices);
    mesh2->AddShader("basic");
    mesh2->Translate(glm::vec3(-1, 0, 0));
    mesh2->SetUp();
    mesh2->GetShader()->SetUniform4f("u_color", 0.0, 1.0, 0.0, 1.0);
    mesh2->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh2->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh2->GetShader()->SetUniformMat4f("u_model", mesh2->GetMatrix());
    mesh2->Clear();

    Mesh *mesh3 = new Mesh("Cube Blue", vertices, indices);
    mesh3->AddShader("basic");
    mesh3->Translate(glm::vec3(1, 0, 2));
    mesh3->SetUp();
    mesh3->GetShader()->SetUniform4f("u_color", 0.0, 0.0, 1.0, 1.0);
    mesh3->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh3->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh3->GetShader()->SetUniformMat4f("u_model", mesh3->GetMatrix());
    mesh3->Clear();

    Mesh *mesh4 = new Mesh("Cube Ref", vertices, indices);
    mesh4->AddShader("basic");
    mesh4->Translate(glm::vec3(-1, 0, 2));
    mesh4->SetUp();
    mesh4->GetShader()->SetUniform4f("u_color", 1.0, 0.0, 0.0, 1.0);
    mesh4->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh4->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh4->GetShader()->SetUniformMat4f("u_model", mesh4->GetMatrix());
    mesh4->Clear();

    Scene* scene = new Scene();
    scene->Init();
    scene->Add(mesh);
    scene->Add(mesh2);
    scene->Add(mesh3);
    scene->Add(mesh4);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();

    renderer->Render();

    return 0;
}