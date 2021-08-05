//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_SHADERMANAGER_H
#define EPSILONEDITOR_SHADERMANAGER_H

#include <string>
#include <TextEditor.h>
#include "ImGuiHandler.h"

namespace Epsilon
{
    class ShaderManager
    {
    public:
        struct UniformData
        {
            float mouse[4];
            float resolution[2];
            float time;
        };
        ShaderManager();


        bool CanRender() const { return program_ != 0;}
        void Update(const UniformData &data, ImGuiHandler &handler);

        ~ShaderManager();

        [[nodiscard]] std::string GetData() const {return data_;}
        void ForceRefresh() { needsUpdate_ = true; }


    private:
        void InitializeShader();

        void InitializeMesh();

        void InitializeUniformBuffer();

        void ApplyUniforms(const UniformData &data);

        void Render(const UniformData &time);


        unsigned program_;
        bool needsUpdate_;
        std::string data_, errMsg_;

        unsigned fragmentHandle_, vertexHandle_;
        unsigned vbo_, vao_, ebo_, ubo_;

        bool modified_;
        float last_;
    };
}

#endif //EPSILONEDITOR_SHADERMANAGER_H
