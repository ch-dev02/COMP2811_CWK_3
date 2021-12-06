//
// Created by twak on 11/11/2019.
//
#include <iostream>
#include "the_player.h"

static int v = 0;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}

// change the image and video for one button every one second
void ThePlayer::shuffle() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
//        setMedia(*i->url);
    buttons -> at( updateCount++ % buttons->size() ) -> init( i );    
}

void ThePlayer::shuffleBtn() {
    TheButtonInfo* i = & infos -> at (rand() % infos->size() );
    setMedia( * i -> url);
    play();
}

void ThePlayer::prevVideo() {
    v--;
    if (v < 0) {
        v = int(infos->size()) - 1;
    } else {
        TheButtonInfo* i = & infos -> at (v);
        setMedia( * i -> url);
        play();
    }
}

void ThePlayer::nextVideo() {
    v++;
    if (v >= int(infos->size())) {
        v = 0;
    } else {
        TheButtonInfo* i = & infos -> at (v);
        setMedia( * i -> url);
        play();
    }    
}

void ThePlayer::seek() {
    setPosition(slider->sliderPosition());
}

void ThePlayer::moveSlider() {
    if (!slider->isSliderDown()) {
        slider->setSliderPosition(position());
    }
}

void ThePlayer::toggleVolumeButton() {
    if (volume() == 0) {
        setVolume(100);
        volumeButton->setIcon(QIcon(":/volume.png"));
        volumeSlider->setSliderPosition(100);
    } else {
        setVolume(0);
        volumeButton->setIcon(QIcon(":/mute.png"));
        volumeSlider->setSliderPosition(0);
    }
}

void ThePlayer::toggleVolumeSlider() {
//    std::cout << volumeSlider->sliderPosition() << std::endl;
    setVolume(volumeSlider->sliderPosition());
    if (volume() == 0) {
        volumeButton->setIcon(QIcon(":/mute.png"));
    } else {
        volumeButton->setIcon(QIcon(":/volume.png"));
    }
}


void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            play(); // starting playing again...
            break;
        case QMediaPlayer::State::PlayingState:
            if (duration() != 0) {
                slider->setRange(0, duration());
//                slider->setSliderPosition(0);
            }
            break;
    default:
        break;
    }
}

void ThePlayer::changeState() {
    switch (state()) {
        case 0: // stopped
            play();
            playButton->setIcon(QIcon(":/pause.png"));
            break;
        case 1: //playing
            pause();
            playButton->setIcon(QIcon(":/play.png"));
            break;
        case 2: //paused
            play();
            playButton->setIcon(QIcon(":/pause.png"));
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);    
    play();
}
