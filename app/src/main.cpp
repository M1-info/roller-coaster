#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Mesh.h"
#include "classes/headers/Scene.h"
#include "classes/headers/Material.h"
#include "classes/headers/OBJLoader.h"

using namespace std;

int main(void)
{
    Renderer* renderer = new Renderer();
    renderer->Init();

    std::vector<float> vertices = {
         0.50,  0.50, -0.50,
         0.50, -0.50, -0.50,
         0.50,  0.50,  0.50,
         0.50, -0.50,  0.50,
        -0.50,  0.50, -0.50,
        -0.50, -0.50, -0.50,
        -0.50,  0.50,  0.50,
        -0.50, -0.50,  0.50,
    };

     std::vector<float> normals = {
         0.0,  1.0,  0.0,
         0.0,  0.0,  1.0,
        -1.0,  0.0,  0.0,
         0.0, -1.0,  0.0,
         1.0,  0.0,  0.0,
         0.0,  0.0, -1.0,
    };

    std::vector<unsigned int> indices = {
        4, 2, 0, 
        2, 7, 3, // front
        6, 5, 7, 
        1, 7, 5, // back
        0, 3, 1, 
        4, 1, 5, // left
        4, 6, 2, 
        2, 6, 7, // right
        6, 4, 5,
        1, 3, 7, // top
        0, 2, 3, 
        4, 0, 1, // bottom
    };

    Mesh *mesh = new Mesh("Cube yellow", vertices, indices, normals);
    mesh->CreateMaterial("phong");
    mesh->GetMaterial()->SetAmbientColor(glm::vec3(1.0, 1.0, 0.0));
    mesh->GetMaterial()->SetDiffuseColor(glm::vec3(1.0, 1.0, 0.0));
    mesh->GetMaterial()->SetSpecularColor(glm::vec3(0.0, 0.0, 1.0));
    mesh->GetMaterial()->SetSpecularExponent(32.0);
    
    // test object loader
    OBJLoader obj("chariot.obj");
    std::cout << obj.ToString() << std::endl;

    // Mesh *mesh2 = new Mesh("Cube Red", vertices, indices, normals);
    // mesh2->CreateMaterial("phong");
    // mesh2->GetMaterial()->SetAmbientColor(glm::vec3(1.0, 1.0, 0.0));
    // mesh2->GetMaterial()->SetDiffuseColor(glm::vec3(1.0, 1.0, 0.0));
    // mesh2->GetMaterial()->SetSpecularColor(glm::vec3(0.0, 0.0, 1.0));
    // mesh2->GetMaterial()->SetSpecularExponent(32.0);
    // mesh2->Translate(glm::vec3(-1, 0, 0));

    // Scene* scene = new Scene();
    // scene->Init();
    // scene->Add(mesh);
    // scene->Add(mesh2);

    // renderer->SetScene(scene);
    // renderer->GetUI()->SetScene(scene);
    // renderer->GetUI()->SetWindow(renderer->GetWindow());
    // renderer->GetUI()->Init();

    // renderer->Render();

    return 0;
}