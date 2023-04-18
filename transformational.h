#ifndef TRANSFORMATIONAL_H
#define TRANSFORMATIONAL_H

#include <QOpenGLFunctions>
#include <QtOpenGL/QtOpenGL>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_1>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>
//class QQuaternion;
//class QVector3D;
//class QMatrix4x4;
//class QOpenGLShaderProgram;
//class QOpenGLFunctions;

class Transformational
{
public:
    virtual ~Transformational() = default;

    virtual void rotate(const QQuaternion &r) = 0; // вращение
    virtual void translate(const QVector3D &t) = 0; // перенос
    virtual void scale(const float &s) = 0; // масштаб
    virtual void setGlobalTransform(const QMatrix4x4 &gt) = 0; // позиция и поворот в лок.сис.коорд. в пределах род.глоб.сис.коорд.
    virtual void draw(QOpenGLShaderProgram* program, QOpenGLShaderProgram *programAxis, QOpenGLFunctions* functions) = 0; // отрисовка
};

#endif // TRANSFORMATIONAL_H
