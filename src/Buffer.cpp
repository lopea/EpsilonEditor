//
// Created by Javier on 9/20/2021.
//

#include <cstring>
#include "Buffer.h"

namespace Epsilon::GL
{
    Buffer::Buffer(GLenum type, GLenum usage, void *data, std::size_t size)
    : type_(type), usage_(usage), size_(size)
    {
        //create a buffer handle
        glGenBuffers(1, &handle_);

        //create a new buffer if necessary
        if((data && size))
        {

            SetData(data, size);
        }
    }

    Buffer::~Buffer()
    {
        //clear any data used
        Invalidate();

        //destroy the buffer
        glDeleteBuffers(1,&handle_);
    }

    void Buffer::Bind() const
    {
        glBindBuffer(type_, handle_);
    }

    void Buffer::Unbind() const
    {
        glBindBuffer(type_, 0);
    }

    void Buffer::Invalidate()
    {
        glInvalidateBufferData(handle_);
    }

    void Buffer::SetData(void *data, std::size_t size, std::size_t offset)
    {
        //check if the parameter data is invalid before continuing
        if(!(data && size) || (offset > 0 && size_ + offset >= size_))
            return;


        //check if the current buffer is empty or needs resizing
        if(!size_ || (size) > size_)
        {
            if(size_)
                Invalidate();

            Bind();

            glBufferData(type_, size_, nullptr, usage_);

            void* buffer = glMapBuffer(type_, GL_WRITE_ONLY);

            if(buffer)
            {
                memcpy(buffer, data, size);
                size_ = size;
                glUnmapBuffer(type_);
            }

            Unbind();
        }
        else
        {
            Bind();
            glBufferSubData(usage_,(GLintptr)offset, (GLsizeiptr)size, data);
            Unbind();
        }

    }

}