//
// Created by Javier on 7/24/2021.
//

#include "App.h"
#include "ImGuiEnvironment.h"

namespace Epsilon
{
    App::App() : window(800, 600)
    {
      ImGuiEnvironment::Initialize();
      ImGuiEnvironment::LinkWindow(&window);
    }

    void App::Run()
    {
      while(!window.WillClose())
      {
        window.ClearFrame();
        ImGuiEnvironment::NewFrame();



        ImGuiEnvironment::Render();

        window.SwapBuffers();
      }

    }

    App::~App()
    {
      ImGuiEnvironment::Shutdown();
    }
}