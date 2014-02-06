/**
 * 
 *      \file irradianceEnvMap.h
 * 
 *      Compute spherical harmonic lighting coefficients for an 
 *      environment map.
 * 
 *      Based on Ravi Ramamoorthi' & Pat Hanrahan's paper 
 * 
 *        "An Efficient Representation for Irradiance Environment Maps"
 * 
 */
 

#pragma once

#ifndef IRRADIANCEENVMAP_HPP
#define IRRADIANCEENVMAP_HPP

#include <glm/glm.hpp>
#include <tools/ImageLoader.hpp>

namespace IrradianceEnvMap
{
  
  void prefilter( const Image_t envmap[6], glm::mat4 M[3]);
  
} //namespace IrradianceEnvMap


#endif //IRRADIANCEENVMAP_HPP
