#include "camera3d.h"
#include <QOpenGLShaderProgram>

Camera3D::Camera3D()
{
    m_scale = 1.0f;
    m_globalTransform.setToIdentity();
}

void Camera3D::draw(QOpenGLShaderProgram *program, QOpenGLShaderProgram *programAxis, QOpenGLFunctions *functions)
{
    if(functions != nullptr) return;

    udateViewMatrix();

    program->setUniformValue("u_viewMatrix", m_viewMatrix);
    //programAxis->setUniformValue("view_matrix", m_viewMatrix);
}

void Camera3D::rotate(const QQuaternion &r)
{
    m_rotate = r * m_rotate;
    udateViewMatrix();
}

void Camera3D::rotateX(const QQuaternion &r)
{
    m_rotateX = r * m_rotateX;
    m_rotate = m_rotateX * m_rotateY;
    udateViewMatrix();
}

void Camera3D::rotateY(const QQuaternion &r)
{
    m_rotateY = r * m_rotateY;
    m_rotate = m_rotateX * m_rotateY;
    udateViewMatrix();
}

void Camera3D::translate(const QVector3D &t)
{
    m_translate += t;
    udateViewMatrix();
}

void Camera3D::scale(const float &s)
{
    m_scale *= s;
    udateViewMatrix();
}

void Camera3D::setGlobalTransform(const QMatrix4x4 &gt)
{
    m_globalTransform = gt;
    udateViewMatrix();
}

const QMatrix4x4 &Camera3D::getViewMatrix() const
{
    return m_viewMatrix;
}

void Camera3D::udateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.translate(m_translate);
    m_viewMatrix.rotate(m_rotate);
    m_viewMatrix.scale(m_scale);
    m_viewMatrix = m_viewMatrix * m_globalTransform.inverted();
}
