#include <vector>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Mesh.h"
#include "classes/headers/Scene.h"
#include "classes/headers/OBJLoader.h"


#include <iostream>
#include <string>
#include <sstream>
using namespace std;

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

    string s = "chariot.obj";
    OBJLoader obj(s);
    cout << obj.ToString() << endl;
    

    /*Mesh *mesh = new Mesh("chario", obj.GetVertices(), obj.GetFaces());
    mesh->AddShader("basic");
    mesh->Translate(glm::vec3(1, 0, 0));
    mesh->SetUp();
    mesh->GetShader()->SetUniform4f("u_color", 1.0, 1.0, 0.0, 1.0);
    mesh->GetShader()->SetUniformMat4f("u_projection", renderer->GetCamera()->GetProjection());
    mesh->GetShader()->SetUniformMat4f("u_view", renderer->GetCamera()->GetView());
    mesh->GetShader()->SetUniformMat4f("u_model", mesh->GetMatrix());
    mesh->Clear();

    Scene* scene = new Scene();
    scene->Init();
    scene->Add(mesh);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();

    renderer->Render();*/

    return 0;
}