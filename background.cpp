#include "background.h"
#include "object3d.h"

Background::Background(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;

    // 12-15
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2),
                               QVector2D(0.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2),
                               QVector2D(0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2),
                               QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2),
                               QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    QVector<GLuint> indexes;

    indexes.append(0);
    indexes.append(2);
    indexes.append(1);
    indexes.append(2);
    indexes.append(3);
    indexes.append(1);

    m_background = new Object3D(vertexes, indexes);//, texture);
}

Background::~Background()
{
    delete m_background;
}

void Background::draw(QOpenGLShaderProgram *program, QOpenGLShaderProgram *programAxis, QOpenGLFunctions *functions)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_background->draw(program, programAxis, functions);
}

void Background::rotate(const QQuaternion &r)
{
    (void)r;
}

void Background::translate(const QVector3D &t)
{
    (void)t;
}

void Background::scale(const float &s)
{
    (void)s;
}

void Background::setGlobalTransform(const QMatrix4x4 &gt)
{
    (void)gt;
}
