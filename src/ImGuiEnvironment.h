//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_IMGUIENVIRONMENT_H
#define EPSILONEDITOR_IMGUIENVIRONMENT_H

#include <imgui.h>

namespace Epsilon
{
    //forward initialization
    class Window;

    class ImGuiEnvironment
    {
    public:
        static void Initialize();

        static bool IsInitialized() {return initialized_;}

        static void NewFrame();

        static void Render();


        static void LinkWindow(Window* window);

        static void Shutdown();
    private:
        static inline bool initialized_ = false;
    };
}

#endif //EPSILONEDITOR_IMGUIENVIRONMENT_H
