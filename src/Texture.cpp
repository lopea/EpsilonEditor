//
// Created by Javier on 8/6/2021.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

namespace Epsilon
{
    static const GLint channels[] =
        {
          GL_R16,
          GL_RG,
          GL_RGB,
          GL_RGBA,
        };
    Texture::Texture(const std::string& location)
    : initialized_(false), reference_(0), wrap_(GL_REPEAT), filter_(GL_NEAREST)
    {
      InitializeTexture();
    }


    void Texture::InitializeTexture()
    {
      //check if the texture has already been initialized,
      if(initialized_)
        return;

      if(location_.empty())
        return;

      //open file location
      FILE* file = nullptr;

      //check if something goes wrong
      if(!(file = fopen(location_.data(), "rb")))
        throw std::runtime_error("Texture Critical Error: Cannot open file!" );



      //get data from stb
      unsigned char* imgData = stbi_load_from_file(file,&width_, &height_, &channels_, 0);

      //cleanup file
      fclose(file);
      //Set buffer data
      glGenTextures(1, &reference_);

      //start adding the texture data to the handle
      glBindTexture(GL_TEXTURE_2D, reference_);

      glTexImage2D(GL_TEXTURE_2D,0, channels[channels_],width_, height_, 0, channels[channels_], GL_UNSIGNED_BYTE, imgData );
      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);

      //free any memory from the texture
      stbi_image_free(imgData);

      //set the texture to be initialized
      initialized_ = true;
    }

    Texture::~Texture()
    {
      if(initialized_)
        Cleanup();
    }

    void Texture::Cleanup()
    {
      glDeleteTextures(1, &reference_);
      //clear initialized flag
      initialized_ = false;
    }

    void Texture::ChangeTextureLocation(const std::string &location)
    {
      if(initialized_)
      {
        Cleanup();
        location_ = location;
        InitializeTexture();
      }
      else
      {
        location_ = location;
      }
    }
}
