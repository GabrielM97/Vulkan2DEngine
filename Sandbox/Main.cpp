#include <iostream>

#include "SandboxApp.h"

int main()
{
    SandboxApp app;

    if (!app.Init())
    {
        std::cerr << "Failed to initialize application.\n";
        return -1;
    }

    app.Run();

    return 0;
}