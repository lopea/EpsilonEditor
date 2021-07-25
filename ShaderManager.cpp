//
// Created by Javier on 7/24/2021.
//

#include "ShaderManager.h"
#include <glad/gl.h>
#include <assert.h>
#include <imgui.h>
#include <gl/gl.h>

#include <iostream>

float verts[] = {
    -1, -1,
    -1, 1,
    1, -1,
    1, 1,
};

unsigned int indices[] = {
    0,2,1,
    3,2,1
    };
namespace Epsilon
{
    //store initial shaders
    const char startVertex [] = "#version 450\nlayout(location = 0) in vec2 position; \
                                layout (location = 0)out vec2 uv; void main(){ uv = vec2(1); gl_Position = vec4(position, 0, 1); }";
    const char startFragment[] = "#version 450\nuniform float time;\n layout(location = 0) in vec2 uv;\nout vec4 fragColor;\nvoid main() {\n\tfragColor = vec4 (0, 1,1,1);\n}";


    ShaderManager::ShaderManager()
    : program(0), needsUpdate_(false), data_(startFragment), errMsg_(), fragmentHandle(0), vertexHandle(0)
    {

      //this feels like cheating...
      editor.SetText(data_);
      editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
      InitializeMesh();
      InitializeShader();
      TextEditor::Palette palette = editor.GetPalette();
      palette[(int)TextEditor::PaletteIndex::Background] = 0xAA000000;
      editor.SetPalette(palette);

    }

    ShaderManager::~ShaderManager()
    {
      glDeleteShader(fragmentHandle);
      glDeleteShader(vertexHandle);
      glDeleteProgram(program);

      unsigned buffs[] = {vbo_,ebo_};
      glDeleteBuffers(2, buffs);

      glDeleteVertexArrays(1, &vao_);
    }

    void ShaderManager::Update(double time)
    {
      ImGui::GetStyle().ChildRounding = 0;
      ImGui::GetStyle().ChildBorderSize = 0;
      Render(time);
      CreateImguiWindow();


      if (data_.size() != editor.GetText().size() || needsUpdate_)
        InitializeShader();

    }

    void ShaderManager::CreateImguiWindow()
    {
      ImGui::SetNextWindowBgAlpha(0.75f);
      ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
      ImGui::SetNextWindowPos({0,0});
      ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
      TextEditor::Palette palette = editor.GetPalette();
      palette[(int)TextEditor::PaletteIndex::Background] = 0x00000000;
      editor.SetPalette(palette);
      editor.Render("render");
      if(!errMsg_.empty())
      {
        ImVec2 errorPos = ImGui::GetIO().DisplaySize;
        errorPos.y *= 0.15f;
        ImGui::SetNextWindowBgAlpha(1.f);
        ImGui::SetNextWindowPos({0, ImGui::GetIO().DisplaySize.y *0.85f});
        ImGui::BeginChild("Error Message",errorPos, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("%s", errMsg_.c_str());
        ImGui::EndChild();
      }
      ImGui::End();
    }

    void ShaderManager::InitializeShader()
    {
      data_ = editor.GetText();
//store the shader handle for the fragment stage
      GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

      //store the reference to the code for the the fragment shader
      GLchar* fragmentData = data_.data();

      //set the code for the fragment shader to the fragment handle
      glShaderSource(fragment,1,&fragmentData, nullptr);

      //complile the shader
      glCompileShader(fragment);

      //check if the shader compiled correctly
      int err;
      glGetShaderiv(fragment, GL_COMPILE_STATUS, &err);

      //if the compilation failed,
      if(!err)
      {
        //get the length of the error message
        int length;
        glGetShaderiv(fragment,GL_INFO_LOG_LENGTH, &length);

        //set the error message to the new length of the error message
        errMsg_.resize(length + 1);

        //get the actual message of the error and store it on to the err string
        glGetShaderInfoLog(fragment, length + 1, &length, errMsg_.data());

        glDeleteShader(fragment);
        //dont do anything
        return;
      }
      errMsg_.clear();
      //update the handle and delete the previous program if necessary
      if(fragmentHandle)
      {
        glDeleteShader(fragmentHandle);
        glDeleteProgram(program);

      }

      fragmentHandle = fragment;

      //create the vertex shader if it hasnt been initialized yet
      if(!vertexHandle)
      {
        //store any errors
        int verr;

        //create the shader handle
        vertexHandle = glCreateShader(GL_VERTEX_SHADER);

        //get the shader code
        const char* v = ((const char*)startVertex);

        //set it to the handle
        glShaderSource(vertexHandle, 1, &v, nullptr);

        //compile the shader
        glCompileShader(vertexHandle);


        //check if it compiled successfully
        glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &verr);
        if(!verr)
        {
          //get the length of the error message
          int length;
          glGetShaderiv(vertexHandle,GL_INFO_LOG_LENGTH, &length);

          //set the error message to the new length of the error message
          errMsg_.resize(length + 1);

          //get the actual message of the error and store it on to the err string
          glGetShaderInfoLog(vertexHandle, length + 1, &length, errMsg_.data());
          std::cerr<< errMsg_ <<std::endl;

        }
        assert(verr);
      }

      //create a new one
      program = glCreateProgram();

      //attach the shader handles
      glAttachShader(program, fragmentHandle);

      //link the shader
      glLinkProgram(program);

      //we have big issues if this does not link
      glGetProgramiv(program, GL_LINK_STATUS, &err);
      assert( err);
    }

    void ShaderManager::Render(double time)
    {
        if(CanRender())
        {
          glUseProgram(program);
          glBindVertexArray(vao_);
          glUniform1f(glGetUniformLocation(program, "time"), (float)time);
          glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, 0);
          glUseProgram(0);
        }
    }

    void ShaderManager::InitializeMesh()
    {
      //create buffer handles for the mesh
      glGenBuffers(1, &vbo_);
      glGenBuffers(1, &ebo_);
      glGenVertexArrays(1, &vao_);

      //bind the array while initializing buffers to that it binds to the array.
      glBindVertexArray(vao_);

      //initialize vertex buffers
      glBindBuffer(GL_ARRAY_BUFFER, vbo_);
      glBufferData(GL_ARRAY_BUFFER,sizeof(float) * _countof(verts), verts, GL_STREAM_DRAW);


      //initialize indices
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _countof(indices), indices, GL_STREAM_DRAW);


      //set attrib
      glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) *2 , nullptr);
      glEnableVertexAttribArray(0);

      //unbind object
      glBindVertexArray(0);
    }
}