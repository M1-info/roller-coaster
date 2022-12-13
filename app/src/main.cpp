#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "classes/headers/VertexArray.h"
#include "classes/headers/Shader.h"
#include "classes/headers/Renderer.h"
#include "classes/headers/Mesh.h"

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Init glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    /* init imgui */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

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


    glm::mat4 projection = glm::perspective(45.0f, 960.f / 540.f, 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Mesh *mesh = new Mesh(vertices.data(), indices.data(), vertices.size() * sizeof(float), indices.size());
    mesh->AddShader("basic");
    mesh->SetUp();
    mesh->GetShader()->SetUniformMat4f("u_projection", projection);
    mesh->GetShader()->SetUniformMat4f("u_view", view);
    mesh->GetShader()->SetUniformMat4f("u_model", model);

    mesh->Clear();

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        renderer.Draw(*mesh->GetVAO(), *mesh->GetIBO(), *mesh->GetShader());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}