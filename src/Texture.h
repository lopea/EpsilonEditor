//
// Created by Javier on 8/6/2021.
//

#ifndef EPSILONEDITOR_TEXTURE_H
#define EPSILONEDITOR_TEXTURE_H

#include <glad/gl.h>
#include <string>

namespace Epsilon
{
    class Texture
    {
    public:
        Texture(const std::string& textureLocation);

        void ChangeTextureLocation(const std::string& location);
        ~Texture();

        void ChangeFilter(GLenum filter);
        void ChangeWrapping(GLenum wrapping);
    private:
        void InitializeTexture();
        void Cleanup();
        bool initialized_;
        GLuint reference_;
        int height_, width_, channels_;
        GLenum wrap_, filter_;
        std::string location_;
    };
}

#endif //EPSILONEDITOR_TEXTURE_H
