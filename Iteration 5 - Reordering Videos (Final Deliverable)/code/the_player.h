//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include <iostream>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QDesktopServices>
#include <QtCore/QDir>
#include <algorithm>
#include <random>
#include <QSlider>
#include <QVBoxLayout>
#include <QtCore/QDirIterator>
#include "the_slider.h"

class ThePlayer : public QMediaPlayer {

Q_OBJECT

private:
    QTimer* mTimer;
    long updateCount = 0;
    void checkButtonStates();
    int count = 0;
    int current = 0;
    bool playing = false;
    bool muted = true;
    void redrawList();
    QTimer* sliderTimer;
public:
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
        sliderTimer = new QTimer(NULL);
        sliderTimer->setInterval(10);
        sliderTimer->start();
        connect( sliderTimer, SIGNAL (timeout()), SLOT ( updateSlider() ) ); // ...running shuffle method

    }

    // all buttons have been setup, store pointers here
    QWidget * mainWindow;
    std::vector<TheButtonInfo>* infos = new std::vector<TheButtonInfo>();
    std::vector<TheButton *>* buttons = new std::vector<TheButton *>();

    // public pointers to control widgets allows
    // us to enable and disable them when necessary
    QPushButton *playPause;
    QPushButton *prevVid;
    QPushButton *nextVid;
    QPushButton *shuffleBtn;
    TheSlider *hs;
    QSlider *vol;
    QPushButton *mute;
    QVBoxLayout *lhs;
    QWidget *scrollWidget;
private slots:

    // change the image and video for one button every one second
    void playStateChanged (QMediaPlayer::State ms);
    void updateSlider();
    void resetSlider(qint64 duration);
    void moveVideoUp(int index);
    void moveVideoDown(int index);
    void removeVideo(int index);
    void renderList();

public slots:
    // slots for control widgets
    void toggleState();
    void toggleMute();
    void nextVideo();
    void prevVideo();
    void setVol(int a);
    // slots for menu buttons
    void openFile();
    void openFolder();
    void downloadExamples();
    void shuffleVideos();
    // change current position in videa
    void updatePosition(int position);
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void switchTo (int index);
};

#endif //CW2_THE_PLAYER_H
