#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

#include "classes/headers/Renderer.h"
#include "classes/headers/Rails.h"
#include "classes/headers/Cart.h"
#include "classes/headers/Scene.h"
#include "classes/headers/Ground.h"

using namespace std;

int main(void)
{
    Renderer *renderer = new Renderer();
    renderer->Init();

    std::shared_ptr<Cart> cart = std::make_shared<Cart>("chariot.obj");

    std::shared_ptr<Ground> island = std::make_shared<Ground>("island.obj");

    std::shared_ptr<Rails> rails = Rails::Create();

    Scene *scene = new Scene();
    scene->Init();
    scene->Add(cart);
    scene->Add(rails);
    scene->Add(island);

    renderer->SetScene(scene);
    renderer->GetUI()->SetScene(scene);
    renderer->GetUI()->SetWindow(renderer->GetWindow());
    renderer->GetUI()->Init();

    renderer->Render();

    return 0;
}