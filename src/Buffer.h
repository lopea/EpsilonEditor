//
// Created by Javier on 9/20/2021.
//

#ifndef EPSILONEDITOR_BUFFER_H
#define EPSILONEDITOR_BUFFER_H


#include <glad/gl.h>
#include <cstddef>

namespace Epsilon::GL
{
    class Buffer
    {
    public:
        Buffer(GLenum type, GLenum usage, void * data = nullptr, std::size_t size = 0);

        virtual ~Buffer();

        void Bind() const;

        void Unbind() const;

        void Invalidate();

        void SetData(void* data, std::size_t size, std::size_t offset = 0);
    private:
        GLenum type_, usage_;
        GLuint handle_;
        std::size_t size_;
    };

}


#endif //EPSILONEDITOR_BUFFER_H
