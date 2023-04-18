#include "object3d.h"

Object3D::Object3D()
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer), // это значение по-умолчанию
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_texture(nullptr)
{
    m_scale = 1.0f;
}

Object3D::Object3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes)//, const QImage &texture)
    : m_vertexBuffer(QOpenGLBuffer::VertexBuffer), // это значение по-умолчанию
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_texture(nullptr)
{
    m_scale = 1.0f;
    init(vertData, indexes);//, texture);
}

Object3D::~Object3D()
{
    free();
}

void Object3D::free()
{
    if(m_vertexBuffer.isCreated()) m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated()) m_indexBuffer.destroy();
//    if(m_texture != nullptr && m_texture->isCreated())
//    { delete m_texture; m_texture = nullptr; }
}

void Object3D::init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes)//, const QImage &texture)
{
    free();

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertData.constData(), vertData.size() * static_cast<int>(sizeof(VertexData)));
    m_vertexBuffer.release();// temp

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * static_cast<int>(sizeof(GLuint)));
    m_indexBuffer.release();// temp

//    m_texture = new QOpenGLTexture(texture.mirrored());
//    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
//    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
//    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLShaderProgram *programShaderAxis, QOpenGLFunctions *functions)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) return;

    QMatrix4x4 modelMatrix;
//    modelMatrix.setToIdentity();
//    modelMatrix.translate(m_translate); // здесь важен порядок преобразований *
//    modelMatrix.rotate(m_rotate); // *
//    modelMatrix.scale(m_scale); // *
//    modelMatrix = m_globalTransform * modelMatrix;

//    m_texture->bind(0);
//    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_modelMatrix", modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

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

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
//    m_texture->release();
}

void Object3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
}

void Object3D::translate(const QVector3D &t)
{
    m_translate += t;
}

void Object3D::scale(const float &s)
{
    m_scale *= s;
}

void Object3D::setGlobalTransform(const QMatrix4x4 &gt)
{
    m_globalTransform = gt;
}
