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

    // Mesh *mesh = new Mesh("Cube yellow", vertices, indices, normals);
    Mesh *mesh = Mesh::FromOBJ("rail.obj");
    Mesh *mesh2 = Mesh::FromOBJ("chariot.obj");

    mesh2->Translate(glm::vec3(3.0f, 0.0f, 0.0f));

    Scene* scene = new Scene();
    scene->Init();
    scene->Add(mesh);
    scene->Add(mesh2);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();

    renderer->Render();

    return 0;
}