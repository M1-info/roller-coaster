#include "Renderer.h"

int main(void)
{
    Renderer *renderer = new Renderer();
    renderer->Init();
    renderer->Render();
    return 0;
}