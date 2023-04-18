#include "mainwindow.h"
#include "window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>

#define VERSION 1.0

MainWindow::MainWindow() :
    open_action(new QAction("Open", this)),
    about_action(new QAction("About", this)),
    quit_action(new QAction("Quit", this)),
    perspective_action(new QAction("Perspective", this)),
    orthographic_action(new QAction("Orthographic", this)),
    shaded_action(new QAction("Shaded", this)),
    wireframe_action(new QAction("Wireframe", this)),
    surfaceangle_action(new QAction("Surface Angle", this)),
    axes_action(new QAction("Draw Axes", this)),
    invert_zoom_action(new QAction("Invert Zoom", this)),
    reload_action(new QAction("Reload", this)),
    autoreload_action(new QAction("Autoreload", this)),
    save_screenshot_action(new QAction("Save Screenshot", this)),
    hide_menuBar_action(new QAction("Hide Menu Bar", this)),
    fullscreen_action(new QAction("Toggle Fullscreen",this)),
    resetTransformOnLoadAction(new QAction("Reset rotation on load",this)),
    recent_files(new QMenu("Open recent", this)),
    recent_files_group(new QActionGroup(this)),
    recent_files_clear_action(new QAction("Clear recent files", this)),
    watcher(new QFileSystemWatcher(this))

{
    //--- настройка вида вывода qDebug() ----
    qSetMessagePattern("[%{time HH:mm:ss.zzz}: %{function}(%{line})]: %{message}");

    setDarkPalette();    //установить темную палитру

    open_action->setShortcut(QKeySequence::Open);
    QObject::connect(open_action, &QAction::triggered, this, &MainWindow::onOpen);
    this->addAction(open_action);
    QObject::connect(about_action, &QAction::triggered, this, &MainWindow::onAbout);


    auto file_menu = menuBar()->addMenu("File");
    file_menu->addAction(open_action);
    file_menu->addMenu(recent_files);
    file_menu->addSeparator();
    file_menu->addAction(reload_action);
    file_menu->addAction(autoreload_action);
    file_menu->addAction(save_screenshot_action);
    file_menu->addAction(quit_action);

    auto view_menu = menuBar()->addMenu("View");
    auto projection_menu = view_menu->addMenu("Projection");
    projection_menu->addAction(perspective_action);
    projection_menu->addAction(orthographic_action);
    auto projections = new QActionGroup(projection_menu);
    for (auto p : {perspective_action, orthographic_action})
    {
        projections->addAction(p);
        p->setCheckable(true);
    }
    projections->setExclusive(true);
    //QObject::connect(projections, &QActionGroup::triggered, this, &MainWindow::on_projection);
    auto draw_menu = view_menu->addMenu("Draw Mode");
    draw_menu->addAction(shaded_action);
    draw_menu->addAction(wireframe_action);
    draw_menu->addAction(surfaceangle_action);
    auto drawModes = new QActionGroup(draw_menu);
    for (auto p : {shaded_action, wireframe_action, surfaceangle_action})
    {
        drawModes->addAction(p);
        p->setCheckable(true);
    }
    drawModes->setExclusive(true);
    //QObject::connect(drawModes, &QActionGroup::triggered, this, &MainWindow::on_drawMode);
    view_menu->addAction(axes_action);
    axes_action->setCheckable(true);
    //QObject::connect(axes_action, &QAction::triggered, this, &MainWindow::on_drawAxes);

    view_menu->addAction(invert_zoom_action);
    invert_zoom_action->setCheckable(true);
    //QObject::connect(invert_zoom_action, &QAction::triggered, this, &MainWindow::on_invertZoom);

    view_menu->addAction(resetTransformOnLoadAction);
    resetTransformOnLoadAction->setCheckable(true);
    //QObject::connect(resetTransformOnLoadAction, &QAction::triggered, this, &MainWindow::on_resetTransformOnLoad);

    view_menu->addAction(hide_menuBar_action);
    //hide_menuBar_action->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    hide_menuBar_action->setCheckable(true);
    //QObject::connect(hide_menuBar_action, &QAction::toggled, this, &MainWindow::on_hide_menuBar);
    this->addAction(hide_menuBar_action);

    view_menu->addAction(fullscreen_action);
    fullscreen_action->setShortcut(Qt::Key_F11);
    fullscreen_action->setCheckable(true);
    //QObject::connect(fullscreen_action, &QAction::toggled, this, &MainWindow::on_fullscreen);
    this->addAction(fullscreen_action);


    auto help_menu = menuBar()->addMenu("Help");
    help_menu->addAction(about_action);

    //load_persist_settings();

    if (!centralWidget())
        setCentralWidget(new Window(this));

    qApp->processEvents();
}

void MainWindow::onOpen()
{
    const QString filename = QFileDialog::getOpenFileName(
                this, "Load .stl file", QString(), "STL files (*.stl *.STL)");
    if (!filename.isNull())
    {

    }

}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "",
        "<p align=\"center\"><b>Fenix3D</b><br>" "1.0" "</p>"
        "<p>3D model viewer<code></code><br>"
        "<a href=\"https://github.com/ShuraFairy/Fenix3D\""
        "   style=\"color: #93a1a1;\">https://github.com/ShuraFairy/Fenix3D</a></p>"
        "<p>© 2023 Shura<br>"
        "<a href=\"petrovai_net@mail.ru\""
        "   style=\"color: #93a1a1;\">petrovai_net@mail.ru</a></p>");
}

//void MainWindow::onAddNew()
//{
//    if (!centralWidget())
//        setCentralWidget(new Window(this));
//    else
//        QMessageBox::information(nullptr, tr("Cannot add new window"),
//                                 tr("Already occupied. Undock first."));
//}

void MainWindow::setDarkPalette()
{
    // Создать палитру для тёмной темы оформления
    QPalette darkPalette;

    QColor base_color = QColor(0, 23, 25); //QColor(0, 38, 43);//QColor(25, 25, 25);
    QColor background_color = QColor(0, 44, 50);//QColor(0, 68, 77); //QColor(53, 53, 53);

    QColor background_color_1 = QColor(26, 89, 97);//QColor(0, 68, 77); //QColor(53, 53, 53);
    QColor text_color = Qt::white;
    QColor text_color_1 = QColor(186, 186, 186);

    QColor highlighted_text_color = Qt::black;//Qt::white;//QColor(69, 157, 131); //
    QColor highlighted_color = QColor(26, 89, 97); //Qt::black;

    //QColor link_color = QColor(99, 181, 188); //QColor(99, 151, 158); //69, 157, 131);
    QColor link_color = QColor(82, 199, 155);//99, 181, 188); //QColor(99, 151, 158); //69, 157, 131); 82 199 155
    QColor light_color = QColor(16, 79, 87);

    QColor placeholder_text_color = QColor(99, 181, 188);
    //QPalette::PlaceholderText
   // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::PlaceholderText,  placeholder_text_color); //цвет фона - не виден

    darkPalette.setColor(QPalette::Window, background_color); //цвет фона - не виден
    darkPalette.setColor(QPalette::WindowText, Qt::white); //цвер текста - не виден

    darkPalette.setColor(QPalette::Base, base_color); //базовый цвет - цвет выпадающих меню
    darkPalette.setColor(QPalette::AlternateBase, background_color);//альтернативный базовый цвет - не виден

    darkPalette.setColor(QPalette::ToolTipBase, text_color); //цвет всплювающей подсказки - не виден
    darkPalette.setColor(QPalette::ToolTipText, text_color);//альтернативный цвет всплювающей подсказки - не виден

//    darkPalette.setColor(QPalette::Foreground, text_color);//цвет текста меток (QLabel)

//    darkPalette.setColor(QPalette::Background, background_color);//цвет фона главного окна и вызываемых окон приложения и меню
    darkPalette.setColor(QPalette::Text, text_color); //цвет текста выпадающих меню

    darkPalette.setColor(QPalette::Button, background_color); //цвет фона кнопок, виджетов, вкладок (с разными оттенками)
    darkPalette.setColor(QPalette::ButtonText, text_color); //цвет текста кнопок, главного меню и маркеров выпадающих меню

    darkPalette.setColor(QPalette::BrightText, Qt::green); //не выден

    darkPalette.setColor(QPalette::Link, link_color); //не виден
    darkPalette.setColor(QPalette::LinkVisited, Qt::red);//не виден

    darkPalette.setColor(QPalette::Highlight, link_color);//base_color); //highlighted_color); //цвет отводки контура кнопок, выделенного текста и текста выбранных пунктов меню
    darkPalette.setColor(QPalette::HighlightedText, highlighted_text_color); //цвет обводки выделенного текста и текста выбранных пунктов меню

    darkPalette.setColor(QPalette::Light, light_color);// link_color); //светлый цвет обводки фрейма (если утоплен или приподнят)
    darkPalette.setColor(QPalette::Dark,  base_color); //темный цвет обводки фрейма (если утоплен или приподнят)
    darkPalette.setColor(QPalette::Shadow, link_color); //Qt::black);//не виден

    QBrush brush = darkPalette.window();
    brush.setColor(brush.color().darker());

    darkPalette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
    darkPalette.setBrush(QPalette::Disabled, QPalette::Text, brush);
    darkPalette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
    darkPalette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    darkPalette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    darkPalette.setBrush(QPalette::Disabled, QPalette::Mid, brush);

    // Установить палитру
    this->setPalette(darkPalette);

//    menuBar->setPalette(darkPalette);
//    menuView->setPalette(darkPalette);
//    menuConfig->setPalette(darkPalette);
//    menuTranslate->setPalette(darkPalette);
//    menuAbout->setPalette(darkPalette);

//    devices_widget->deviceTree_widget->poup_menu->setPalette(darkPalette);
//    devices_widget->deviceTree_widget->addDeviceSubMenu->setPalette(darkPalette);
//    devices_widget->deviceTree_widget->addPortSubMenu->setPalette(darkPalette);
//    devices_widget->deviceTree_widget->addInterfaceSubMenu->setPalette(darkPalette);

//    devices_widget->dmx_widget->poup_menu->setPalette(darkPalette);
//    devices_widget->dmx_widget->addRgbSubMenu->setPalette(darkPalette);

     //******** 2021.06.17
//    devices_widget->mdi_widget->mdi_num_comboBox->setPalette(darkPalette);
//    devices_widget->mdi_widget->mdi_num_comboBox->view()->setPalette(darkPalette);
//    devices_widget->mdo_widget->mdo_num_comboBox->setPalette(darkPalette);
//    devices_widget->mdo_widget->mdo_num_comboBox->view()->setPalette(darkPalette);
//    devices_widget->dmx_widget->dmx_num_comboBox->setPalette(darkPalette);
//    devices_widget->dmx_widget->dmx_num_comboBox->view()->setPalette(darkPalette);
    //******
//    devices_widget->setPalette(darkPalette);
//    devices_widget->plc_widget->setPalette(darkPalette);
//    devices_widget->plc_widget->setDarkPalette(true);
//    devices_widget->plc_widget->bridge_widget->bridgeNum_comboBox->setPalette(darkPalette);
//    devices_widget->plc_widget->bridge_widget->bridgeNum_comboBox->view()->setPalette(darkPalette);
//    devices_widget->plc_widget->indicator_widget->num_comboBox->setPalette(darkPalette);
//    devices_widget->plc_widget->indicator_widget->num_comboBox->view()->setPalette(darkPalette);

//    time_label->setPalette(darkPalette);

//    this->devices_widget->setStyleSheet("QToolTip{background-color: rgb(0, 23, 25); color: white; border: 1px solid rgb(26, 89, 97);}");

}
