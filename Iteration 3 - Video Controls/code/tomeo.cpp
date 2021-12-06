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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QSlider>
#include <QAbstractSlider>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include "the_player.h"
#include "the_button.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);


    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // ------------------------------------------------------------------------------
    QWidget *controlsWidget = new QWidget();
    QPushButton *playButton = new QPushButton();
    playButton->setIcon(QIcon(":/pause.png"));;
    QPushButton *prevButton = new QPushButton();
    prevButton->setIcon(QIcon(":/prev.png"));
    QPushButton *nextButton = new QPushButton();
    nextButton->setIcon(QIcon(":/next.png"));
    QPushButton *volumeButton = new QPushButton();
    volumeButton->setIcon(QIcon(":/volume.png"));
    QPushButton *shuffleButton = new QPushButton();
    shuffleButton->setIcon(QIcon(":/shuffle.png"));

    player->playButton = playButton;
    player->volumeButton = volumeButton;

//    QLabel *LabelDuration = new QLabel("00:00");
    QSlider* slider = new QSlider(Qt::Horizontal);
    player->slider= slider;
    slider->setRange(0, 30);
    slider->setSliderPosition(0);

    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    player->volumeSlider= volumeSlider;
    volumeSlider->setRange(0, 100);
    volumeSlider->setSliderPosition(100);

    slider->connect(slider, SIGNAL(sliderReleased()), player, SLOT(seek()));
    playButton->connect(playButton, SIGNAL(clicked()), player, SLOT (changeState()));
    shuffleButton->connect(shuffleButton, SIGNAL(clicked()), player, SLOT (shuffleBtn()));
    prevButton->connect(prevButton, SIGNAL(clicked()), player, SLOT (prevVideo()));
    nextButton->connect(nextButton, SIGNAL(clicked()), player, SLOT (nextVideo()));

    volumeButton->connect(volumeButton, SIGNAL(clicked()), player, SLOT (toggleVolumeButton()));
    volumeSlider->connect(volumeSlider, SIGNAL(sliderReleased()), player, SLOT(toggleVolumeSlider()));

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsWidget->setLayout(controlsLayout);
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(shuffleButton);
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(nextButton);    
    controlsLayout->addWidget(slider);
    controlsLayout->addWidget(volumeButton);
    controlsLayout->addWidget(volumeSlider);

    // ------------------------------------------------------------------------------


    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QMainWindow *main = new QMainWindow;
    player->mainWindow = main;
    QWidget window;
    QHBoxLayout *menuBarLayout = new QHBoxLayout();
    QMenuBar *menuBar = new QMenuBar();
    QMenu *files = menuBar->addMenu("File");
    files->addAction("Open File");
    files->addAction("Open Folder");
    files->addAction("Exit");

    menuBar->setLayout(menuBarLayout);

    volumeSlider->setMaximumWidth(main->width()*0.2);

    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(menuBar);
    top->addWidget(videoWidget);
    top->addWidget(controlsWidget);
    top->addWidget(buttonWidget);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
