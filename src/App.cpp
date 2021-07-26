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

        UpdateData();
        manager_.Update(data);
        ImGuiEnvironment::Render();

        window.SwapBuffers();
      }

    }

    App::~App()
    {
      ImGuiEnvironment::Shutdown();
    }

    void App::UpdateData()
    {
      //get values from window
      const int* res = window.GetResolution();
      const int* mouse = window.GetMouse();
      const int* pos = window.GetPosition();

      //update x and y coordinates for both mouse and resolution
      for(int i = 0 ; i < 2; i ++)
      {
        data.resolution[i] = (float)res[i];
        //update mouse to be in normalized window space
        float newValue = (float)mouse[i];
        newValue -= (float)pos[i];
        newValue /= data.resolution[i];
        data.mouse[i] = newValue;

        //update mouse clicks
        data.mouse[2] = (float)mouse[2];
        data.mouse[3] = (float)mouse[3];
      }

      //flip y value for mouse
      data.mouse[1] = data.resolution[1] - data.mouse[1];

      //update time
      data.time = (float)window.GetCurrentTime();

    }
}