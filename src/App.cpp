//
// Created by Javier on 7/24/2021.
//

#include "App.h"
#include "ImGuiEnvironment.h"

namespace Epsilon
{
    App::App() : window(1024, 768)
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

        manager_.Update(window.GetCurrentTime());
        ImGuiEnvironment::Render();

        window.SwapBuffers();
      }

    }

    App::~App()
    {
      ImGuiEnvironment::Shutdown();
    }
}