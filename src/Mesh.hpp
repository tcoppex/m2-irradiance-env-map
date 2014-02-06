/**
 * 
 *    \file Mesh.hpp
 * 
 *     \todo : Placer la geometrie GPU (VAO, VBO,..) dans une structure et
 *             liée Matrice + BufferObject dans un même ensemble.
 *             Donc simplifier le concept de 'Mesh' à l'objet seul, sans 
 *             transformation.
 * 
 */
 

#pragma once

#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <GLType/VertexBuffer.hpp>



/** MESH ---------------------------------------------- */

class Mesh
{
  protected:
    bool m_bInitialized;    
    
    VertexBuffer m_vertexBuffer;
    GLsizei m_count;
    
    /* TODO Move in another object */
    glm::mat4 m_model;
    glm::mat3 m_normal; // 'kind of' the model matrix (different for the scale)
                        // each time the model matrix is change, it has to change too !
                        // (for lighted objects)
    
    
  public:
    Mesh()
      : m_bInitialized(false)
    {}
    
    virtual ~Mesh() {}
    
    virtual void init() {}
    virtual void draw() const {}
    
    void setModelMatrix(const glm::mat4 &model)     {m_model = model;}
    void setNormalMatrix(const glm::mat3 &normal)   {m_normal = normal;}
    
    const glm::mat4& getModelMatrix() const   {return m_model;}
    const glm::mat3& getNormalMatrix() const  {return m_normal;}
};


/** PLANE MESH ----------------------------------------- */

class PlaneMesh : public Mesh
{
  protected:
    float m_size;
    
  public:
    PlaneMesh(float size=100.0f)//
      : Mesh(),
        m_size(size)
    {}
    
    void init();
    void draw() const;
};


/** SPHERE MESH ---------------------------------------- */

class SphereMesh : public Mesh
{
  protected:
    int m_meshResolution;
    float m_radius;
    
  public:
    SphereMesh(int res=8, float radius=1.0f)
      : Mesh(),
        m_meshResolution(res),
        m_radius(radius)
    {}
    
    void init();
    void draw() const;
};


/** CONE MESH ------------------------------------------ */

class ConeMesh : public Mesh
{
  public:
    ConeMesh()
      : Mesh()
    {}
    
    void init();
    void draw() const;
};


/** CUBE MESH ------------------------------------------ */

class CubeMesh : public Mesh
{
  public:
    CubeMesh()
      : Mesh()
    {}
    
    void init();
    void draw() const;
};


#endif //MESH_HPP
