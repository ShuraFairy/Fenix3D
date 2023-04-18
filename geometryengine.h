#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>
//#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "data.h"
#include "loader.h"

class Mesh
{
public:

    Mesh();
    Mesh(QVector<VertexData> &vertData, QVector<GLuint> &indexes);
    void draw(QOpenGLShaderProgram * program, QOpenGLFunctions* functions);
    void init(QVector<VertexData> &vertData, QVector<GLuint> &indexes);
    Mesh(const Mesh_ & m_);
    Mesh(Mesh_ && m_) noexcept;

protected:
    void free();

private:
public:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture * m_texture;

    QVector3D m_diffuseColor;
    QVector3D m_ambienceColor;
    QVector3D m_specularColor;

    QVector<VertexData> vertexes;
    QVector<GLuint> indexes;
};

class Model
{    

public:
    Model() { }; 
    void draw(QOpenGLShaderProgram * program, QOpenGLFunctions* functions);
    void exportModel();
    const QVector3D & getSceneCenter();
    float getScale();
    Model(const Model_ & model_);
    Model &operator=(const Model_ & model_);
    Model(Model_ && model_) noexcept;

private:
public:
    std::vector<Mesh> meshes;

private:

    Assimp::Importer import;
    Assimp::Exporter exporter;
    const aiScene *scene;
    aiScene * export_scene;

public:
    aiVector3D scene_min, scene_max, scene_center;    
    QVector3D center;
    float scale;
};


#endif // GEOMETRYENGINE_H
