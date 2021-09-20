//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_SHADER_H
#define EPSILONEDITOR_SHADER_H

#include <string>
#include <TextEditor.h>
#include "ImGuiHandler.h"

namespace Epsilon
{
    class UniformDataBlock;
    class Shader
    {
    public:
        Shader();


        [[nodiscard]] bool CanRender() const { return program_ != 0;}
        void Update(const UniformDataBlock &data, ImGuiHandler &handler);

        ~Shader();

        [[nodiscard]] std::string GetData() const {return data_;}
        void ForceRefresh() { needsUpdate_ = true; }

        static const char* GetStartShader();

    private:
        void InitializeShader();

        void InitializeMesh();

        void InitializeUniformBuffer();

        void ApplyUniforms(const UniformDataBlock &data);

        void Render(const UniformDataBlock &time);


        unsigned program_;
        bool needsUpdate_;
        std::string data_, errMsg_;

        unsigned fragmentHandle_, vertexHandle_;
        unsigned vbo_, vao_, ebo_, ubo_;

        bool modified_;
        float last_;
    };
}

#endif //EPSILONEDITOR_SHADER_H
