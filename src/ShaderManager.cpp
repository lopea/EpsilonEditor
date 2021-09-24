//
// Created by Javier on 7/24/2021.
//

#include "ShaderManager.h"
#include "UniformData.h"

#include <glad/gl.h>
#include <cassert>
#include <imgui.h>
#include <gl/GL.h>

#include <iostream>

float verts[] = {
    -1, -1, 0,0,
    -1, 1, 0,1,
    1, -1, 1,0,
    1, 1, 1,1
};

unsigned int indices[] = {
    0,2,1,
    3,2,1
    };
namespace Epsilon
{
    //store initial shaders
    const char startVertex [] = "#version 330\n\
out InData\n\
{\
vec2 uv;\
vec2 outpos;\
}data;\
layout (location = 0) in vec2 position;\
layout (location = 1) in vec2 uv;\
void main() {\n\
gl_Position = vec4(position, 0, 1);\
data.outpos = position;\
data.uv = uv;\
}";
    const char startFragment[] = "#version 450\nlayout(std140, binding = 0) uniform UniformData\n{\n\tvec4 mouse;\n\tvec2 resolution;\n\tfloat time;\n};\nin InData\n{\n  vec2 uv;\n  vec2 outpos;\n};\nout vec4 fragColor;\nvoid main()\n{\n\tvec2 u = -1. + 2. * uv;\n\tvec2 r = cos(2./(u.x) * atan(u.x, u.y) + time) - sin( u+ 1./length((u )));\n\tfragColor = vec4(r.yxxy);\n}";
    ShaderManager::ShaderManager()
    : program_(0), needsUpdate_(false), data_(startFragment), fragmentHandle_(0), vertexHandle_(0)
    {
      

      //setup data for the shader renderer
      InitializeMesh();
      InitializeShader();
      InitializeUniformBuffer();


    }

    ShaderManager::~ShaderManager()
    {
      glDeleteShader(fragmentHandle_);
      glDeleteShader(vertexHandle_);
      glDeleteProgram(program_);

      unsigned buffs[] = {vbo_, ebo_, ubo_};
      glDeleteBuffers(2, buffs);

      glDeleteVertexArrays(1, &vao_);

    }

    void ShaderManager::Update(const UniformData &data, ImGuiHandler &handler)
    {


      Render(data);

      if (data_.size() != handler.GetEditorCount() || needsUpdate_)
      {
        data_ = handler.GetEditorString();
        InitializeShader();
        handler.SetErrorText(errMsg_);
      }
    }

    void ShaderManager::InitializeShader()
    {

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

      //update the handle and delete the previous program_ if necessary
      if(fragmentHandle_)
      {
        glDeleteShader(fragmentHandle_);
        glDeleteProgram(program_);
      }

      fragmentHandle_ = fragment;

      //create the vertex shader if it hasnt been initialized yet
      if(!vertexHandle_)
      {
        //store any errors
        int verr;

        //create the shader handle
        vertexHandle_ = glCreateShader(GL_VERTEX_SHADER);

        //get the shader code
        const char* v = ((const char*)startVertex);

        //set it to the handle
        glShaderSource(vertexHandle_, 1, &v, nullptr);

        //compile the shader
        glCompileShader(vertexHandle_);


        //check if it compiled successfully
        glGetShaderiv(vertexHandle_, GL_COMPILE_STATUS, &verr);
        if(!verr)
        {
          //get the length of the error message
          int length;
          glGetShaderiv(vertexHandle_,GL_INFO_LOG_LENGTH, &length);

          //set the error message to the new length of the error message
          errMsg_.resize(length + 1);

          //get the actual message of the error and store it on to the err string
          glGetShaderInfoLog(vertexHandle_, length + 1, &length, errMsg_.data());
          std::cerr<< errMsg_ <<std::endl;

        }
        assert(verr);
      }

      //create a new one
      program_ = glCreateProgram();

      //attach the shader handles
      glAttachShader(program_, fragmentHandle_);

      glAttachShader(program_, vertexHandle_);

      //link the shader
      glLinkProgram(program_);

      //we have big issues if this does not link
      glGetProgramiv(program_, GL_LINK_STATUS, &err);

      //cant link shader, send message
      if(!err)
      {
        //get length of the info log
        int length;

        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);


        //set the size for the error message
        errMsg_.resize(length + 1);

        //get the message
        glGetProgramInfoLog(program_, length + 1 , &length, errMsg_.data());

      }
    }

    void ShaderManager::Render(const UniformData &data)
    {
        if(CanRender())
        {
          ApplyUniforms(data);
          glUseProgram(program_);
          glBindVertexArray(vao_);
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
      glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, nullptr);
      glEnableVertexAttribArray(0);

      //uv attrib
      glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2));
      glEnableVertexAttribArray(1);

      //unbind object
      glBindVertexArray(0);
    }

    void ShaderManager::ApplyUniforms(const UniformData &data)
    {
      //send the current uniform data to the shader
      glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
      glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(UniformData), &data);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);

      glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_);
    }

    void ShaderManager::InitializeUniformBuffer()
    {
      //create handle for the uniform buffer
      glGenBuffers(1, &ubo_);

      //intialize buffer data
      glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData), nullptr, GL_DYNAMIC_DRAW);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);


    }

    const char *ShaderManager::GetStartShader()
    {
      return startFragment;
    }

}