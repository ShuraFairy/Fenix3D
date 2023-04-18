#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"

#include <QGroupBox>
#include <QFileDialog>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QPainter>


Window::Window(MainWindow *mw)
    : mainWindow(mw),
      drawMode(true)
{
    QGroupBox * pToolsGroupBox  = new QGroupBox(tr("Tools"));
    QGroupBox * pSettingsGroupBox  = new QGroupBox(tr("Settings"));
    QVBoxLayout * pvertToolsLayout  = new QVBoxLayout;
    QVBoxLayout * pvertSettingsLayout  = new QVBoxLayout;    

    openFileBtn = new QPushButton(tr("Open File"), this);
    exportBtn = new QPushButton(tr("Export Model"), this);
    loadModelBtn = new QPushButton(tr("Import Model"), this);
    loadModelBtn->setFixedWidth(120);

    pvertToolsLayout->addWidget(openFileBtn);
    pvertToolsLayout->addWidget(exportBtn);
    pvertToolsLayout->addWidget(loadModelBtn);
    pvertToolsLayout->addStretch(1);
    pToolsGroupBox->setLayout(pvertToolsLayout);
    pToolsGroupBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    settingBtn = new QPushButton(tr("Settings"), this);
    drawModeBtn = new QPushButton(tr("Draw Mode"), this);
    label = new QLabel(tr(""), this);
    settingBtn->setFixedWidth(120);
    pvertSettingsLayout->addWidget(settingBtn);
    pvertSettingsLayout->addWidget(drawModeBtn);
    pvertSettingsLayout->addWidget(label);
    pvertSettingsLayout->addStretch(1);
    pSettingsGroupBox->setLayout(pvertSettingsLayout);
    pSettingsGroupBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    glWidget = new GLWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;    
    container->addWidget(glWidget);
    container->addWidget(pToolsGroupBox);
    container->addWidget(pSettingsGroupBox);    

    QWidget *w = new QWidget;    
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);

    setWindowTitle(tr("Fenix3D"));
    glWidget->set_drawMode(normal);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
    timer->start(1000); // И запустим таймер

    connect(exportBtn, &QPushButton::clicked, glWidget, &GLWidget::exportModel);
    connect(openFileBtn, SIGNAL(clicked()), SLOT(OnButtonOpenFileClicked()));
    connect(drawModeBtn, SIGNAL(clicked()), SLOT(OnButtonDrawModeClicked()));
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::OnButtonOpenFileClicked()
{
    std::string fileNameImport = "";

    fileNameImport = QFileDialog::getOpenFileName(this, ("Open File"), "", tr("Models (*.fbx *.obj *.stl)")).toStdString();

    glWidget->loadModel(fileNameImport);
    //glWidget->initModel(fileNameImport);

    emit fileChanged(fileNameImport);
}

void Window::OnButtonDrawModeClicked()
{
    if(drawMode == true)
    {
        glWidget->set_drawMode(wireframe);
        drawMode = false;
    }
    else
    {
        glWidget->set_drawMode(normal);
        drawMode = true;
    }
}

void Window::slotTimer()
{
    label->setText(QTime::currentTime().toString("hh:mm:ss"));  
}


