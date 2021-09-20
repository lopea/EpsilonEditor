//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_APP_H
#define EPSILONEDITOR_APP_H
#include "Window.h"
#include "Shader.h"
#include "UniformDataBlock.h"

namespace Epsilon
{
    class App
    {
    public:
        App();

        void Run();

        ~App();

    private:
        //! Update any uniform data for the current frame
        void UpdateData();


        //! References the window that will show the shader on the screen
        Window window;

        //! Stores the shader that will get sent to the screen
        Shader manager_;

        //! Stores and handles any imgui data that will be used as proper UI.
        ImGuiHandler imgui_;
    };
}

#endif //EPSILONEDITOR_APP_H
