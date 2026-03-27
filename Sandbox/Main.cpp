#include <iostream>
#include <GLFW/glfw3.h>
#include "Core/Application.h"

int main()
{
    Application app;

    if (!app.Init())
    {
        std::cerr << "Failed to initialize application.\n";
        return -1;
    }

    app.Run();

    return 0;
}
