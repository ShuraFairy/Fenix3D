#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QLabel>

class QPushButton;
class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

protected:
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    //void onOpen();

private slots:
    void OnButtonOpenFileClicked();
    void OnButtonDrawModeClicked();
    void slotTimer();

signals:
    void fileChanged(std::string file);

private:
    GLWidget    * glWidget;
    QPushButton * loadModelBtn;
    QPushButton * exportBtn;
    QPushButton * openFileBtn;
    QPushButton * settingBtn;
    QPushButton * drawModeBtn;
    MainWindow  * mainWindow;
    QLabel      * label;

    QTimer *timer;
    bool drawMode;
};

#endif
