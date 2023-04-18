#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyle>
#include <QStyleFactory>
#include <QStylePainter>
#include <QPainter>
#include <QPalette>
#include <QApplication>
#include <QTranslator>
#include <QTimer>
#include <QTime>
#include <QActionGroup>
#include <QFileSystemWatcher>
#include <QCollator>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void onOpen();
    void onAbout();
//    void on_bad_stl();
//    void on_empty_mesh();
//    void on_missing_file();

//    void enable_open();
//    void disable_open();

//    void set_watched(const QString& filename);


private slots:
//    void on_projection(QAction* proj);
//    void on_drawMode(QAction* mode);
//    void on_drawAxes(bool d);
//    void on_invertZoom(bool d);
//    void on_resetTransformOnLoad(bool d);
//    void on_watched_change(const QString& filename);
//    void on_reload();
//    void on_autoreload_triggered(bool r);
//    void on_clear_recent();
//    void on_load_recent(QAction* a);
//    void on_loaded(const QString& filename);
//    void on_save_screenshot();
//    void on_fullscreen();
//    void on_hide_menuBar();

public:
    void setDarkPalette(); ///установить темную тему

private:    
    QAction* const open_action;
    QAction* const about_action;
    QAction* const quit_action;
    QAction* const perspective_action;
    QAction* const orthographic_action;
    QAction* const shaded_action;
    QAction* const wireframe_action;
    QAction* const surfaceangle_action;
    QAction* const axes_action;
    QAction* const invert_zoom_action;
    QAction* const reload_action;
    QAction* const autoreload_action;
    QAction* const save_screenshot_action;
    QAction* const hide_menuBar_action;
    QAction* const fullscreen_action;
    QAction* const resetTransformOnLoadAction;

    QMenu* const recent_files;
    QActionGroup* const recent_files_group;
    QAction* const recent_files_clear_action;

    QString current_file;
    QString lookup_folder;
    QStringList lookup_folder_files;

    QFileSystemWatcher* watcher;
};

#endif
