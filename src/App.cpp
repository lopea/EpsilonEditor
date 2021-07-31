//
// Created by Javier on 7/24/2021.
//

#include "App.h"
#include "ImGuiEnvironment.h"
#include <filesystem>
#include <iostream>
#include <fstream>
namespace Epsilon
{
    App::App() : window(1024, 768)
    {
      //check if a shader has been saved
      if(std::filesystem::exists("last.glsl"))
      {
        //open the file
        std::ifstream file ("last.glsl" , std::ios_base::binary );

        //get contents of the file
        std::vector<char> fileData((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));


        manager_.SetData(std::string(fileData.begin(),fileData.end()));
      }
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
      //open previous file shader
      std::ofstream prevShaderFile("last.glsl");
      if(prevShaderFile.is_open())
      {
        prevShaderFile << manager_.GetData();
      }
      else
      {
        std::cerr << "EPSILON ERROR: Unable to open last.glsl!!!! Loss of data is expected!!!" << std::endl;
      }
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
        data.mouse[i + 2] = (float)mouse[i + 2];
      }

      //flip y value for mouse
      data.mouse[1] = data.resolution[1] - data.mouse[1];

      //update time
      data.time = (float)window.GetCurrentTime();

    }
}