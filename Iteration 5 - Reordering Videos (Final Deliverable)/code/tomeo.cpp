//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//  2811 cw3 : twak 11/11/2021
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include <QScrollArea>

int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // create the main window, layout, menu bar and actions
    QMainWindow * main = new QMainWindow;
    player->mainWindow = main;
    QWidget * window = new QWidget;
    main->setCentralWidget(window);

    QAction* openFile = new QAction("Open File");
    auto fileMenu = main->menuBar()->addMenu("&File");
    fileMenu->addAction(openFile);
    QAction* openFolder = new QAction("Open Folder");
    fileMenu->addAction(openFolder);
    QAction* downloadExamples = new QAction("Download Examples");
    fileMenu->addAction(downloadExamples);
    openFile->connect(openFile, SIGNAL (triggered()), player, SLOT (openFile()));
    openFolder->connect(openFolder, SIGNAL (triggered()), player, SLOT (openFolder()));
    downloadExamples->connect(downloadExamples, SIGNAL(triggered()), player, SLOT(downloadExamples()) );

    QVBoxLayout *rhs = new QVBoxLayout();
    player->lhs = new QVBoxLayout();
    player->lhs->setAlignment(Qt::AlignTop);

    QScrollArea *lhScroll = new QScrollArea();
    player->scrollWidget = new QWidget();
    lhScroll->setWidget(player->scrollWidget);
    lhScroll->setWidgetResizable(true);
    player->scrollWidget->setLayout(player->lhs);
    lhScroll->setMaximumWidth(250);

    QHBoxLayout *top = new QHBoxLayout();
    window->setLayout(top);
    main->setWindowTitle("tomeo");
    main->setMinimumSize(800, 680);

    // create the video control buttons
    // controls layout
    QHBoxLayout *controls = new QHBoxLayout();
    //play pause button
    player->playPause = new QPushButton();
    player->playPause->setIcon(QIcon(":/play.png"));
    player->playPause->connect(player->playPause, SIGNAL (clicked()), player, SLOT(toggleState()));
    player->playPause->setDisabled(true);
    player->playPause->setMaximumWidth(30);

    // previous video button
    player->prevVid = new QPushButton();
    player->prevVid->setIcon(QIcon(":/prev.png"));
    player->prevVid->setDisabled(true);
    player->prevVid->connect(player->prevVid, SIGNAL (clicked()), player, SLOT(prevVideo()));
    player->prevVid->setMaximumWidth(30);

    // shuffle button
    player->shuffleBtn = new QPushButton();
    player->shuffleBtn->setIcon(QIcon(":/shuffle.png"));
    player->shuffleBtn->setDisabled(true);
    player->shuffleBtn->connect(player->shuffleBtn, SIGNAL (clicked()), player, SLOT(shuffleVideos()));
    player->shuffleBtn->setMaximumWidth(30);

    // next video button
    player->nextVid = new QPushButton();
    player->nextVid->setIcon(QIcon(":/next.png"));
    player->nextVid->setDisabled(true);
    player->nextVid->connect(player->nextVid, SIGNAL (clicked()), player, SLOT(nextVideo()));
    player->nextVid->setMaximumWidth(30);

    // time scrubber
    player->hs = new TheSlider();
    player->hs->setDisabled(true);
    player->connect(player, SIGNAL (durationChanged(qint64)), player, SLOT (resetSlider(qint64)));
    player->connect(player, SIGNAL (positionChanged(qint64)), player, SLOT(updateSlider(qint64)));
    player->hs->connect(player->hs, SIGNAL (sliderMoved(int)), player, SLOT (updatePosition(int)));

    // volume scrubber
    player->vol = new QSlider(Qt::Horizontal);
    player->vol->setDisabled(true);
    player->vol->setMaximumWidth(100);
    player->mute = new QPushButton();
    player->mute->setIcon(QIcon(":/volume.png"));
    player->mute->setMaximumWidth(30);
    player->mute->setDisabled(true);
    player->mute->connect(player->mute, SIGNAL (clicked()), player, SLOT (toggleMute()));
    player->vol->connect(player->vol, SIGNAL (valueChanged(int)), player, SLOT (setVol(int)));

    controls->addWidget(player->playPause);
    controls->addWidget(player->prevVid);
    controls->addWidget(player->nextVid);
    controls->addWidget(player->shuffleBtn);
    controls->addStretch();
    controls->addWidget(player->mute);
    controls->addWidget(player->vol);

    // add the video and the buttons to the right hand side widget
    rhs->addWidget(videoWidget);
    rhs->addWidget(player->hs);
    rhs->addLayout(controls);

    // add left and right hand side layouts to top layout
    top->addWidget(lhScroll);
    top->addLayout(rhs);

    // showtime!
    main->show();

    // wait for the app to terminate
    return app.exec();
}
