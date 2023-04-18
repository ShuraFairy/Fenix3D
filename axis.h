#ifndef AXIS_H
#define AXIS_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
//#include <QOpenGLFunctions_3_1>
//#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLContext>

class Axis : protected QOpenGLFunctions
{
public:
    Axis();
    void setScale(QVector3D min, QVector3D max);
    void draw(QOpenGLShaderProgram *programAxis, QOpenGLFunctions*functions,
              QMatrix4x4 transMat, QMatrix4x4 orientMat, QMatrix4x4 aspectMat, float aspectRatio);
              //QMatrix4x4 transMat, QMatrix4x4 viewMat);//, /*QMatrix4x4 orientMat, QMatrix4x4 aspectMat,*/ float aspectRatio);
private:
    // QOpenGLShaderProgram shader;
    QOpenGLBuffer vertices, // GL Buffer for model-space coords
        flowerAxisVertices; // GL Buffer for hud-space axis lines
    QOpenGLBuffer flowerLabelVertices[3];//Buffer for hud-space label lines
};


#endif // AXIS_H
