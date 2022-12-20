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
         0, 1, 2, 
         0, 2, 3, 
         4, 5, 6, 
         4, 6, 7, 
         0, 1, 4, 
         4, 5, 1, 
         2, 3, 7, 
         2, 7, 6,
         1, 2, 6, 
         1, 6, 5,
         0, 3, 7, 
         0, 7, 4,
    };


    Mesh *mesh = new Mesh("Cube", vertices, indices);
    mesh->AddShader("basic");
    mesh->Translate(glm::vec3(-1, 0, 0));
    mesh->SetUp();
    mesh->GetShader()->SetUniform4f("u_color", 1.0, 1.0, 0.0, 1.0);
    mesh->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh->GetShader()->SetUniformMat4f("u_model", mesh->GetMatrix());
    mesh->Clear();

    Mesh *mesh2 = new Mesh("Cube2", vertices, indices);
    mesh2->AddShader("basic");
    mesh2->Translate(glm::vec3(1, 0, 0));
    mesh2->SetUp();
    mesh2->GetShader()->SetUniform4f("u_color", 0.0, 1.0, 0.0, 1.0);
    mesh2->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh2->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh2->GetShader()->SetUniformMat4f("u_model", mesh2->GetMatrix());
    mesh2->Clear();

    Scene* scene = new Scene();
    scene->Add(mesh);
    scene->Add(mesh2);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();

    renderer->Render();

    return 0;
}