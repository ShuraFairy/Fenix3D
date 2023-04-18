#ifndef OBJECT3D_H
#define OBJECT3D_H

#include<QOpenGLBuffer>
#include<QMatrix4x4>
#include<QVector2D>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions>
#include<QOpenGLTexture>

#include "transformational.h"

#include "data.h"
//struct VertexData
//{
//    VertexData() {}
//    VertexData(QVector3D p, QVector2D t, QVector3D n)
//        : position(p), textcoord(t), normal(n) {}
//    QVector3D position;
//    QVector2D textcoord;
//    QVector3D normal;
//};

class Object3D : public Transformational
{
public:
    Object3D();
    Object3D(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes);//, const QImage &texture);
    virtual ~Object3D();

    void init(const QVector<VertexData> &vertData, const QVector<GLuint> &indexes);//, const QImage &texture);
    void draw(QOpenGLShaderProgram * program, QOpenGLShaderProgram *programShaderAxis, QOpenGLFunctions* functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &gt);

protected:
    void free();

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLTexture* m_texture;
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
};


#endif // OBJECT3D_H
