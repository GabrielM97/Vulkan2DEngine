#include <iostream>

#include "PongApp.h"

int main()
{
    PongApp app;

    if (!app.Init())
    {
        std::cerr << "Failed to initialize application.\n";
        return -1;
    }

    app.Run();

    return 0;
}