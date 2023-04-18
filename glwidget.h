#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QThread>
#include <memory>

#include "geometryengine.h"
#include "loader.h"

class Background;
class Camera3D;
class Axis;

enum class DrawMode {normal, shaded, wireframe, surfaceangle, DRAWMODECOUNT};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);    
    ~GLWidget();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void initModel(const std::string & pathToModelFile);
    void set_drawMode(enum DrawMode mode);
    void loadModel(const std::string & path);    

private slots:    

public slots:

    void loadedModel(bool complete);
    void cleanup();
    void exportModel();

signals:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
    void initShaders();
    void draw_axes(bool d);

private:
    QMatrix4x4 orient_matrix() const;
    QMatrix4x4 transform_matrix() const;
    QMatrix4x4 aspect_matrix() const;
    void resetTransform();

    QOpenGLShader* mesh_vertshader;
    QOpenGLShaderProgram m_programShaderAxis;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programBackground;
    QOpenGLShaderProgram m_programWireframeShader;
    QOpenGLTexture * m_texture;

    static bool m_transparent;

    QMatrix4x4 currentTransform;
    QMatrix4x4 m_projectionMatrix;
    QVector2D m_mousePosition;

    std::unique_ptr<Model_> model_;
    std::vector<Model*> m_objects;

    Background * m_back;    
    Camera3D * m_camera;
    Axis* m_axis;
    enum class DrawMode drawMode;
    bool drawAxes = true;
};

#endif
