#include "glwidget.h"

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QThread>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "camera3d.h"
#include "background.h"
#include "axis.h"

bool GLWidget::m_transparent = false;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = new Camera3D;    
    m_camera->translate(QVector3D(0.0f, 0.0f, -2.0f));
//    currentTransform = QMatrix4x4();
//    resetTransform();
}

GLWidget::~GLWidget()
{
    cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::cleanup()
{
    makeCurrent();
    delete m_camera;
    delete m_back;
    delete m_axis;

    doneCurrent();
    QObject::disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
}

void GLWidget::exportModel()
{
    if(!m_objects.empty())
        m_objects.at(0)->exportModel();
}

void GLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    initShaders();    
    //initModel();

    m_back = new Background(1000.0);
    m_axis = new Axis();
}

void GLWidget::paintGL()
{
    //glEnable(GL_CULL_FACE); // отсечение невидимиых треугольников 

    QOpenGLShaderProgram* selected_mesh_shader = nullptr;

    bool mode = true;
    if(drawMode == wireframe)
    {
        mode = false;
        selected_mesh_shader = &m_programWireframeShader;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(drawMode == normal)
    {
        mode = true;
        selected_mesh_shader = &m_program;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_programShaderAxis.bind();
    m_programShaderAxis.setUniformValue("transform_matrix", m_projectionMatrix);

    m_programBackground.bind();
    m_programBackground.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_programBackground.setUniformValue("u_drawMode", mode);
    m_back->draw(&m_programBackground, &m_programShaderAxis, context()->functions());
    //m_programBackground.release();

    selected_mesh_shader->bind();
    selected_mesh_shader->setUniformValue("u_projectionMatrix", m_projectionMatrix);
    selected_mesh_shader->setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    selected_mesh_shader->setUniformValue("u_lightPower", 1.3f);

    //m_camera->draw(&m_program);
    m_camera->draw(selected_mesh_shader, &m_programShaderAxis);


    if(m_objects.size() > 0)
        for(auto obj : m_objects) obj->draw(selected_mesh_shader, context()->functions());

    //if (drawAxes)
    if(m_objects.size() > 0)
        m_axis->draw(&m_programShaderAxis, context()->functions(),
                     transform_matrix(), orient_matrix(), aspect_matrix(), width() / float(height()));
    //transform_matrix(), view_matrix(), /*orient_matrix(), aspect_matrix(),*/
    //         width() / float(height()));

    m_programBackground.release();
    m_programShaderAxis.release();
    selected_mesh_shader->release();
}

void GLWidget::resizeGL(int w, int h)
{
    float aspect = GLfloat(w) / h;  // Calculate aspect ratio
    // Set near plane to 0.1, far plane to 700.0, field of view 45 degrees
    const float zNear = 0.10f;
    const float zFar = 3000.0f;
    const float fov = 45.0f;
    m_projectionMatrix.setToIdentity(); // Reset projection
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);   // Set perspective projection
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        m_mousePosition = QVector2D(event->position().toPoint());// localPos());
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QVector2D diffpos = QVector2D(event->position().toPoint()) - m_mousePosition;
        m_mousePosition = QVector2D(event->position().toPoint());
        float angleX = diffpos.y() / 2.0f;
        float angleY = diffpos.x() / 2.0f;
        m_camera->rotateX(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, angleX));
        m_camera->rotateY(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, angleY));

        update();
    }
    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
    {        
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.15f));
    }
    else if (event->angleDelta().y() < 0)
    {        
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.15f));
    }

    update();
}

void GLWidget::initShaders()
{
    mesh_vertshader = new QOpenGLShader(QOpenGLShader::Vertex);
    if(!mesh_vertshader->compileSourceFile(":/vshader.glsl"))
        close();

    if(!m_program.addShader(mesh_vertshader))
        close();
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!m_program.link())    // Link shader pipeline
        close();
    if (!m_program.bind())    // Bind shader pipeline for use
        close();

    if(!m_programBackground.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vbackground.glsl"))
        close();
    if(!m_programBackground.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fbackground.glsl"))
        close();
    if (!m_programBackground.link())    // Link shader pipeline
        close();
    if (!m_programBackground.bind())    // Bind shader pipeline for use
        close();

    if(!m_programWireframeShader.addShader(mesh_vertshader))
        close();
    if(!m_programWireframeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/mesh_wireframe.glsl"))
        close();
    if (!m_programWireframeShader.link())    // Link shader pipeline
        close();
    if (!m_programWireframeShader.bind())    // Bind shader pipeline for use
        close();

    if(!m_programShaderAxis.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcolored_lines.glsl"))
        close();
    if(!m_programShaderAxis.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcolored_lines.glsl"))
        close();
    if(!m_programShaderAxis.link())      // Link shader pipeline
        close();
    if (!m_programShaderAxis.bind())    // Bind shader pipeline for use
        close();
}

void GLWidget::initModel(const std::string & pathToModelFile)
{
    //m_objects.append(new Model(pathToModelFile));
}

void GLWidget::set_drawMode(DrawMode mode)
{
    drawMode = mode;
    update();
}

void GLWidget::loadModel(const std::string &path)
{
    QThread *thread= new QThread;
    model_ = std::make_unique<Model_>();
    model_->path_ = path;
    model_->moveToThread(thread);

    connect(model_.get(), SIGNAL(loaded(bool)), this, SLOT(loadedModel(bool)));
    connect(thread, SIGNAL(started()), model_.get(), SLOT(loadModel_()));

    thread->start();

    qDebug() << "end load model";
}

void GLWidget::loadedModel(bool complete)
{
    qDebug() << "end loaded model";
    qDebug() << "count meshes: " << model_->meshes.size();

    Model * model = new Model(*model_.get());
    m_objects.push_back(model);

    QVector3D lower(model->scene_min.x, model->scene_min.y, model->scene_min.z);
    QVector3D upper(model->scene_max.x, model->scene_max.y, model->scene_max.z);
    qDebug() << "lower: " << lower;
    qDebug() << "upper: " << upper;
    m_axis->setScale(lower, upper);
    update();
}

void GLWidget::draw_axes(bool d)
{
    drawAxes = d;
    update();
}

QMatrix4x4 GLWidget::orient_matrix() const
{
    QMatrix4x4 m = currentTransform;
    return m;
}
QMatrix4x4 GLWidget::transform_matrix() const
{
    QMatrix4x4 m = orient_matrix();
    m.scale(model_->scale);// scale);
    m.translate(-model_->center);// -center);
    return m;
}
QMatrix4x4 GLWidget::aspect_matrix() const
{
    QMatrix4x4 m;
    if (width() > height())
    {
        m.scale(-height() / float(width()), 1, 0.5);
    }
    else
    {
        m.scale(-1, width() / float(height()), 0.5);
    }
    return m;
}

void GLWidget::resetTransform()
{
    currentTransform.setToIdentity();
    // apply some rotations to define initial orientation
    currentTransform.rotate(-90.0, QVector3D(1, 0, 0));
    currentTransform.rotate(180.0 + 15.0, QVector3D(0, 0, 1));
    currentTransform.rotate(15.0, QVector3D(1, -sin(M_PI/12), 0));

//    zoom = 1;
}



