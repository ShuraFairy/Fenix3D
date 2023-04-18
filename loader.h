#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_3_1>
//#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "data.h"

class Mesh_
{
public:

    Mesh_();
    Mesh_(QVector<VertexData> &vertData, QVector<GLuint> &indexes);
    Mesh_(const Mesh_ & m_);

public:
    QVector3D m_diffuseColor;
    QVector3D m_ambienceColor;
    QVector3D m_specularColor;

public:
    QVector<VertexData> vertexes_;
    QVector<GLuint> indexes_;
};

class Model_ : public QObject
{
    Q_OBJECT
public:
    Model_();    

private:
public:
    std::vector<Mesh_> meshes;

signals:
    void loaded(bool complete);

public slots:
    void loadModel_();

private:
    void processNode(aiNode *node, const aiScene *scene);
    Mesh_ processMesh(aiMesh *mesh, const aiScene *scene);
    void get_bounding_box(aiVector3D* min, aiVector3D* max);
    void get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);

    Assimp::Importer import;
    Assimp::Exporter exporter;
    const aiScene *scene;
    aiScene * export_scene;

public:

    aiVector3D scene_min, scene_max, scene_center;
    QVector3D center;
    QVector3D lower;
    QVector3D upper;
    float scale;
    bool complete = false;

    std::string path_;
};

#endif // LOADER_H
