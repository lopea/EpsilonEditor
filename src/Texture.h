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
        Texture();
        void InitializeTexture();
        ~Texture();
        void ChangeFilter(GLenum filter);
        void ChangeWrapping(GLenum wrapping);
    private:
        bool initialized_;
        GLuint reference_;
        GLenum wrap_, filter_;
        std::string location_;
    };
}

#endif //EPSILONEDITOR_TEXTURE_H
