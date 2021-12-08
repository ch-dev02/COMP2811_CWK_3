//
// Created by twak on 11/11/2019.
//
#include <iostream>
#include "the_player.h"
#include <QFileDialog>
using namespace std;
// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
//    if(*infos.size()>0)
//        jumpTo(buttons -> at(0) -> info);

    int n=buttons->size();

    std::cout<<"no. of buttons "<<n;
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
//    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
//    buttons -> at( updateCount++ % buttons->size() ) -> init( i );
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    std::cout<<"Button pressed"<<std::endl;
//    QString filename=QFileDialog::getOpenFileName(button,"Open a File","","Video File (*.avi,*.mpg,*.mp4)");
    setMedia( * button -> url);
//    setMedia(QUrl::fromLocalFile(filename));
    play();
}


void ThePlayer::playVideo(QString filename) {
    std::cout<<"Playing Video  ";
//    QString filename=QFileDialog::getOpenFileName(this,"Open a File","","Video File (*.avi,*.mpg,*.mp4)");
    setMedia(QUrl::fromLocalFile(filename));
    play();
}
