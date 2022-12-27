#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Mesh.h"
#include "classes/headers/Scene.h"
#include "classes/headers/Rails.h"
#include "classes/headers/Material.h"
#include "classes/headers/OBJLoader.h"

using namespace std;

int main(void)
{
    Renderer* renderer = new Renderer();
    renderer->Init();

    // Mesh *mesh = Mesh::FromOBJ("rail.obj");
    // Mesh *mesh2 = Mesh::FromOBJ("chariot.obj");
    Mesh *mesh3 = Mesh::FromOBJ("cube.obj");

    // mesh2->Translate(glm::vec3(3.0f, 0.0f, 0.0f));

    std::vector<glm::vec3> controlPoints;
    controlPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    controlPoints.push_back(glm::vec3(6.0f, 2.0f, 0.0f));
    controlPoints.push_back(glm::vec3(12.0f, -2.0f, 0.0f));
    controlPoints.push_back(glm::vec3(18.0f, 0.0f, 0.0f));
    controlPoints.push_back(glm::vec3(24.0f, 3.0f, 0.0f));
    controlPoints.push_back(glm::vec3(18.0f, -4.0f, 0.0f));
    controlPoints.push_back(glm::vec3(20.0f, -2.0f, 0.0f));
    controlPoints.push_back(glm::vec3(32.0f, 0.0f, 0.0f));
    controlPoints.push_back(glm::vec3(32.0f, 0.0f, 0.0f));
    controlPoints.push_back(glm::vec3(32.0f, 0.0f, 0.0f));


    Rails *rails = new Rails();
    rails->CreateFromControlPoints(controlPoints);

    Scene* scene = new Scene();
    scene->Init();
    // scene->Add(mesh);
    // scene->Add(mesh2);
    // scene->Add(mesh3);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();
    renderer->SetRails(rails);


    renderer->Render();

    return 0;
}