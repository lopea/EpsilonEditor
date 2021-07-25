//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_APP_H
#define EPSILONEDITOR_APP_H
#include "Window.h"
namespace Epsilon
{
    class App
    {
    public:
        App();

        void Run();

        ~App();

    private:
        Window window;
    };
}

#endif //EPSILONEDITOR_APP_H
