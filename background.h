#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "transformational.h"

#include <QOpenGLFunctions>
//#include <QtOpenGL/QtOpenGL>
#include <QOpenGLShaderProgram>
//#include <QOpenGLFunctions_3_1>
//#include <QOpenGLFunctions_4_5_Core>
//include <QOpenGLContext>
//#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>

class Object3D;

class Background : public Transformational//, protected QOpenGLFunctions_3_1
{
public:
    Background(float width);
    virtual ~Background();
    void draw(QOpenGLShaderProgram* program, QOpenGLShaderProgram *programAxis = nullptr, QOpenGLFunctions*functions = nullptr);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &gt);

private:
    Object3D* m_background;

};

#endif // BACKGROUND_H
