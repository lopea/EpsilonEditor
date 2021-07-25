//
// Created by Javier on 7/24/2021.
//

#include "App.h"

namespace Epsilon
{
    App::App() : window(800, 600)
    {

    }

    void App::Run()
    {
      while(!window.WillClose())
      {
        window.ClearFrame();

        window.SwapBuffers();
      }

    }

    App::~App()
    {

    }
}