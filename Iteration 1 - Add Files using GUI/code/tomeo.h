#ifndef TOMEO_H
#define TOMEO_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSlider>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionPlay_triggered();

    void on_actionStop_triggered();

    void on_actionPause_triggered();

    void on_Pause_clicked();

    void on_Stop_clicked();

    void on_Play_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QVideoWidget* vw;
    QProgressBar* bar;
    QSlider* slider;
};
#endif // TOMEO_H
