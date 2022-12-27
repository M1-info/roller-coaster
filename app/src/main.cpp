#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Rails.h"
#include "classes/headers/Cart.h"
#include "classes/headers/Scene.h"

using namespace std;

int main(void)
{
    Renderer* renderer = new Renderer();
    renderer->Init();

    std::shared_ptr<Cart> cart = std::make_shared<Cart>("chariot.obj");

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

    std::shared_ptr<Rails> rails = std::make_shared<Rails>(controlPoints);

    Scene* scene = new Scene();
    scene->Init();
    scene->Add(cart);
    scene->Add(rails);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();


    renderer->Render();

    return 0;
}