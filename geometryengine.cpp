#include "geometryengine.h"

#include <iostream>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QTimer>
#include <vector>

#define aisgl_min(x, y) (x < y ? x : y)
#define aisgl_max(x, y) (y > x ? y : x)


Mesh::Mesh()
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_texture(nullptr)
{ }

Mesh::Mesh(QVector<VertexData> &vertData, QVector<GLuint> &indexes)
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_texture(nullptr)

{    
    init(vertData, indexes);
}

void Mesh::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;

    m_vertexBuffer.bind();

    int offset = 0;

    qDebug() << m_vertexBuffer.size();
    int vertloc = program->attributeLocation("a_position");
    program->enableAttributeArray(vertloc);
    program->setAttributeBuffer(vertloc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texloc = program->attributeLocation("a_textcoord");
    program->enableAttributeArray(texloc);
    program->setAttributeBuffer(texloc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normloc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normloc);
    program->setAttributeBuffer(normloc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    qDebug() << m_indexBuffer.size();    
    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
}

void Mesh::init(QVector<VertexData> &vertData, QVector<GLuint> &indexes)
{
    free();

    qDebug() << "vertData: " << vertData.size();

    m_vertexBuffer.create();
    if ( !m_vertexBuffer.bind() )
    {
        qWarning() << "Could not bind vertex buffer to the context";        
    }

    m_vertexBuffer.allocate(vertData.data(), vertData.size() * static_cast<int>(sizeof(VertexData)));
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.data(), indexes.size() * static_cast<int>(sizeof(GLuint)));
    m_indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indexBuffer.release();

    qDebug() << "end init mesh";
}

void Mesh::free()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
}

Mesh::Mesh(const Mesh_ &m_)
{
    vertexes = m_.vertexes_;
    indexes = m_.indexes_;
}

Mesh::Mesh(Mesh_ &&m_) noexcept :
    vertexes(std::move(m_.vertexes_)),
    indexes(std::move(m_.indexes_))
{

}

void Model::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{    
    qDebug() << "start draw model";

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    scale = getScale();
    center = getSceneCenter();
    modelMatrix.scale(scale);
    modelMatrix.translate(-center.x(), -center.y(), -center.z());
    qDebug() << "scale: " << scale;
    qDebug() << "center: " << center;

    program->setUniformValue("u_modelMatrix", modelMatrix);

    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(program, functions);

    qDebug() << "end draw model";
}

void Model::exportModel()
{
    qDebug() << "Export model";

    export_scene = const_cast<aiScene*>(scene);
    auto ret = exporter.Export(export_scene, "stl", "testExport.stl");

    if(ret == aiReturn_FAILURE)
        qDebug() << "Export Failure";
    else if(ret == aiReturn_SUCCESS)
        qDebug() << "Export Success";
}

const QVector3D &Model::getSceneCenter()
{
    QVector3D lower(scene_min.x, scene_min.y, scene_min.z);
    QVector3D upper(scene_max.x, scene_max.y, scene_max.z);
    center = (lower + upper) / 2;
    return center;
}

float Model::getScale()
{
    QVector3D lower(scene_min.x, scene_min.y, scene_min.z);
    QVector3D upper(scene_max.x, scene_max.y, scene_max.z);
    scale = 2 / (upper - lower).length();
    return scale;
}

Model::Model(const Model_&model_)
{
    meshes.resize(model_.meshes.size());

    for(size_t i = 0; i < model_.meshes.size(); ++i)
    {
        meshes.at(i).indexes = model_.meshes.at(i).indexes_;
        meshes.at(i).vertexes = model_.meshes.at(i).vertexes_;
        meshes.at(i).init(meshes.at(i).vertexes, meshes.at(i).indexes);
    }
    center = model_.center;
    scale = model_.scale;
    scene_max = model_.scene_max;
    scene_min = model_.scene_min;
    scene_center = model_.scene_center;
}

Model &Model::operator=(const Model_ &model_)
{
    meshes.resize(model_.meshes.size());

    for(size_t i = 0; i < model_.meshes.size(); ++i)
    {
        meshes.at(i).indexes = model_.meshes.at(i).indexes_;
        meshes.at(i).vertexes = model_.meshes.at(i).vertexes_;
        meshes.at(i).init(meshes.at(i).vertexes, meshes.at(i).indexes);
    }
    center = model_.center;
    scale = model_.scale;
    scene_max = model_.scene_max;
    scene_min = model_.scene_min;
    scene_center = model_.scene_center;

    return *this;
}

Model::Model(Model_ &&model_) noexcept //: meshes(std::move(model_.meshes))
{

}
