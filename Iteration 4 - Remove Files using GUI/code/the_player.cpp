//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

void ThePlayer::switchTo(int index){
    current = index;
    jumpTo(&infos->at(index));
}

void ThePlayer::resetSlider(qint64 duration){
    qDebug() << duration;
    int length = static_cast<int>(duration);
    this->hs->setMaximum(length);
    this->hs->setMinimum(0);
    this->hs->setValue(0);
    this->hs->setSingleStep(1);
}

void ThePlayer::updateSlider(){
    this->hs->setValue(position());
}

void ThePlayer::updatePosition(int position){
    qint64 p = static_cast<qint64>(position);
    if(p != this->position()){
        this->setPosition(p);
    }
}

void ThePlayer::setVol(int a){
    this->setVolume(a);
    if(a == 0){
        this->mute->setIcon(QIcon(":/volume.png"));
    }else{
        this->mute->setIcon(QIcon(":/mute.png"));
    }
}

void ThePlayer::nextVideo(){
    int index = 1;
    int prevCurrent = current;

    while(current < count-1){ // while not at end of vector
        current += index;
        if(QString::compare(infos->at(current).url, QString(""), Qt::CaseInsensitive) != 0){
            jumpTo(&infos->at(current));
            break;
        }
    }
    if(QString::compare(infos->at(current).url, QString(""), Qt::CaseInsensitive) == 0){
        current = prevCurrent;
    }

}

void ThePlayer::prevVideo(){
    int index = -1;
    int prevCurrent = current;
    while(current > 0){ // while not at start of vector
        current += index;
        if(QString::compare(infos->at(current).url, QString(""), Qt::CaseInsensitive) != 0){
            jumpTo(&infos->at(current));
            break;
        }
    }
    if(QString::compare(infos->at(current).url, QString(""), Qt::CaseInsensitive) == 0){
        current = prevCurrent;
    }
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

void ThePlayer::toggleState(){
    if(playing == true){
        pause();
        this->playPause->setIcon(QIcon(":/play.png"));
        playing = false;
    }else{
        play();
        this->playPause->setIcon(QIcon(":/pause.png"));
        playing = true;
    }
}

void ThePlayer::toggleMute(){
    if(muted == true){
        setVolume(100);
        this->mute->setIcon(QIcon(":/mute.png"));
        muted = false;
        this->vol->setValue(100);
    }else{
        setVolume(0);
        this->mute->setIcon(QIcon(":/volume.png"));
        muted = true;
        this->vol->setValue(0);
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia(QUrl(QUrl::fromLocalFile(button->url)));
    play();
    pause();
    playing = false;
    this->playPause->setIcon(QIcon(":/play.png"));
}

void ThePlayer::openFile(){
    QString filename =  QFileDialog::getOpenFileName(
                this->mainWindow,
                "Open File",
                QDir::currentPath(),
            #if defined(_WIN32)
                "Video Files (*.wmv)"); // windows
#else
                "Video Files (*.MOV *.mp4)");
#endif

    if( !filename.isNull() )
    {
#if defined(_WIN32)
        if (filename.contains(".wmv"))  { // windows
#else
        if (filename.contains(".mp4") || filename.contains("MOV"))  { // mac/linux
#endif
            infos->push_back(TheButtonInfo(filename));
            //free(this->infos); // free memory from deep copied vector
            this->count += 1;
            TheButton * btn = new TheButton(QUrl(filename).fileName(), this->count-1);
            btn->connect(btn, SIGNAL (leftClicked(int)), this, SLOT(switchTo(int)));
            this->connect(btn, SIGNAL (moveVideoUp(int)), this, SLOT(moveVideoUp(int)));
            this->connect(btn, SIGNAL (moveVideoDown(int)), this, SLOT(moveVideoDown(int)));
            this->connect(btn, SIGNAL (removeVideo(int)), this, SLOT(removeVideo(int)));
            this->lhs->addWidget(btn);
            this->buttons->push_back(btn);
        }
    }

    if(infos->size() == 1){
        jumpTo(&infos->at(0));
        current = 0;
    }
    checkButtonStates();
}

void ThePlayer::checkButtonStates(){
    if(count == 0 || infos->size() == 0){ // when no videos loaded disable controls
        this->playPause->setDisabled(true);
        this->prevVid->setDisabled(true);
        this->nextVid->setDisabled(true);
        this->hs->setDisabled(true);
        this->vol->setDisabled(true);
        this->mute->setDisabled(true);
    }else{
        this->playPause->setDisabled(false);
        this->prevVid->setDisabled(false);
        this->nextVid->setDisabled(false);
        this->hs->setDisabled(false);
        this->vol->setDisabled(false);
        this->mute->setDisabled(false);
    }
}

void ThePlayer::openFolder(){
    QString path =  QFileDialog::getExistingDirectory(
                this->mainWindow,
                "Open Folder",
                QDir::currentPath(),
                QFileDialog::ShowDirsOnly);

    if( !path.isNull() )
    {
        this->count = 0;
        infos->clear();
        QDir dir(path);
        QDirIterator it(dir);

        while (it.hasNext()) { // for all files

            QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
                if (f.contains(".wmv"))  { // windows
#else
                if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif
                    infos->push_back(TheButtonInfo(f)); // add to the output list
                    TheButton * btn = new TheButton(QUrl(f).fileName(), this->count);
                    btn->connect(btn, SIGNAL (leftClicked(int)), this, SLOT(switchTo(int)));
                    this->connect(btn, SIGNAL (moveVideoUp(int)), this, SLOT(moveVideoUp(int)));
                    this->connect(btn, SIGNAL (moveVideoDown(int)), this, SLOT(moveVideoDown(int)));
                    this->connect(btn, SIGNAL (removeVideo(int)), this, SLOT(removeVideo(int)));
                    this->lhs->addWidget(btn);
                    this->buttons->push_back(btn);
                    this->count += 1;
                }
        }
    }
    if(infos->size() != 0){
        jumpTo(&infos->at(0));
        current = 0;
    }
    checkButtonStates();
}

void ThePlayer::downloadExamples(){
    QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
}

void ThePlayer::removeVideo(int index){
    infos->at(index).url = QString("");
    renderList();
}

void ThePlayer::renderList(){
    for(int i = 0; i<buttons->size(); i++){
        buttons->at(i)->deleteLater();
    }

    buttons = new std::vector<TheButton *>();
    for(int i = 0; i<infos->size(); i++){
        QString f = infos->at(i).url;
        if(QString::compare(f, QString(""), Qt::CaseInsensitive) != 0){
            TheButton * btn = new TheButton(QUrl(f).fileName(), i);
            btn->connect(btn, SIGNAL (leftClicked(int)), this, SLOT(switchTo(int)));
            this->connect(btn, SIGNAL (moveVideoUp(int)), this, SLOT(moveVideoUp(int)));
            this->connect(btn, SIGNAL (moveVideoDown(int)), this, SLOT(moveVideoDown(int)));
            this->connect(btn, SIGNAL (removeVideo(int)), this, SLOT(removeVideo(int)));
            this->lhs->addWidget(btn);
            this->buttons->push_back(btn);
        }
    }
}
