/**
 * 
 *        ImageLoader.hpp
 * 
 *    Simple FreeImage wrapper to load 2D & Rectangle image for OpenGL
 *    (stored as unsigned char). 
 *  
 *    TODO : rewrite it completely to handle multiples internal format
 */
 

#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <GL/glew.h>
#include <FreeImage/FreeImage.h>
#include <cassert>
#include <cstdio>
#include <iostream>


struct Image_t
{
  unsigned int bytesPerPixel;
  
  GLenum target;  
  GLint internalFormat;
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLenum type;
  GLubyte *data;  
  
  
  Image_t()
    : target(GL_INVALID_ENUM),
      internalFormat(0),
      width(0), 
      height(0),
      format(GL_INVALID_ENUM),
      type(GL_INVALID_ENUM),
      data(0)
  {
   	// when using FreeImage as a static library
    #ifdef FREEIMAGE_LIB
      //FreeImage_Initialise();
    #endif
  }
  
  virtual ~Image_t()
  { 
    clean(); 
    
    // when using FreeImage as a static library
    #ifdef FREEIMAGE_LIB
      //FreeImage_DeInitialise();
    #endif
  }
  
  void clean()
  {
    if (data != 0) {
      delete [] data;
    }
  }
  
  bool load(const char *filename)
  {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType( filename,0);
    FIBITMAP* image = FreeImage_Load( format, filename);
    
    if (image == 0)
    {
      std::cerr << "ImageLoader : "<<filename << " can't be founded."<<std::endl;
      return false;
    }
    
    if (setDefaultAttributes(image) == false)
    {    
      // Convert to 32 bits
      FIBITMAP* tmp = image;
      image = FreeImage_ConvertTo32Bits(image);
      FreeImage_Unload(tmp);
      
      if (setDefaultAttributes(image) == false)
      {
        std::cerr << "ImageLoader : "<<filename << " can't be loaded."<<std::endl;
        return false;
      }
    }
   
    clean();
    
    data = new GLubyte[bytesPerPixel*width*height];
    unsigned char* bits = (unsigned char*)FreeImage_GetBits(image);

    const size_t numPixels = width*height;
    
    // FreeImage loads in BGR format, so we need to swap the RED & BLUE bytes (or use GL_BGR).
    // Furthermore, the pixel order is invert to match openGL implementation	
    if (bytesPerPixel == 4)
    {
      const size_t end = 4u*(numPixels-1u);
      
      for (size_t j=0u; j<numPixels; ++j)
      {
        size_t idx = 4u*j;
        data[idx+0u] = bits[end-idx+2u];
        data[idx+1u] = bits[end-idx+1u];
        data[idx+2u] = bits[end-idx+0u];
        data[idx+3u] = bits[end-idx+3u];
      }
    } 
    else if (bytesPerPixel == 3)
    {
      const size_t end = 3u*(numPixels-1u);
      
      for (size_t j=0u; j<numPixels; ++j)
      {
        size_t idx = 3u*j;
        data[idx+0u] = bits[end-idx+2u];
        data[idx+1u] = bits[end-idx+1u];
        data[idx+2u] = bits[end-idx+0u];
      }
    }

    FreeImage_Unload(image);
    
    return true;
  }
  
  private:
    Image_t(const Image_t&);
    const Image_t& operator=(const Image_t&) const;
    
  
    bool setDefaultAttributes(FIBITMAP *dib)
    { 
      if (FIT_BITMAP != FreeImage_GetImageType(dib)) {
        return false;
      }
      
      unsigned int bpp = FreeImage_GetBPP(dib);
      
      switch (bpp)
      {      
        case 8u:
          internalFormat = format = GL_RED;
        break;
        
        case 16u:
          internalFormat = format = GL_RG;
        break;
        
        case 24u:
          internalFormat = format = GL_RGB;
        break;
        
        case 32u:
          internalFormat = format = GL_RGBA;
        break;
        
        default:
          return false;
      }
      
      bytesPerPixel = bpp / 8u;
      width = FreeImage_GetWidth(dib);
      height = FreeImage_GetHeight(dib);
      target = (width==height)? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE;
      type = GL_UNSIGNED_BYTE;
      
      return true;
    }
  
};

#endif //IMAGELOADER_HPP
