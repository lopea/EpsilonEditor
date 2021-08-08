//
// Created by Javier on 8/6/2021.
//

#include "Texture.h"
#include <
namespace Epsilon
{
    Texture::Texture() : initialized_(false), reference_(0), wrap_(GL_REPEAT), filter_(GL_NEAREST) {}

    void Texture::InitializeTexture()
    {
      //check if the texture has already been initialized,
      if(initialized_)
        return;

      //Set buffer data
      glGenTextures(1, &reference_);

      glBindTexture(GL_TEXTURE_2D, reference_);


      glBindTexture(GL_TEXTURE_2D, 0);

      //set the texture to be initialized,
      initialized_ = true;
    }
}
