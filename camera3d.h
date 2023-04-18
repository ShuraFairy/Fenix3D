#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "transformational.h"

#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>

class Camera3D : public Transformational
{
public:
    Camera3D();
    virtual ~Camera3D() = default;

    void draw(QOpenGLShaderProgram* program, QOpenGLShaderProgram *programAxis = nullptr, QOpenGLFunctions*functions = nullptr);
    void rotate(const QQuaternion &r);
    void rotateX(const QQuaternion &r);
    void rotateY(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &gt);
    const QMatrix4x4 & getViewMatrix() const;

protected:
    void udateViewMatrix();

private:
    QQuaternion m_rotate;
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
public:
    QMatrix4x4 m_viewMatrix;
};

#endif // CAMERA3D_H
