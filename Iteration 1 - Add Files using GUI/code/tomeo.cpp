#include "tomeo.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("tomeo");
    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    player->setVideoOutput(vw);
    this->setCentralWidget(vw);

    slider = new QSlider(this);
    bar = new QProgressBar(this);

    slider->setOrientation(Qt::Horizontal);

    QPushButton* p = new QPushButton(this);
    ui->statusBar->addWidget(p);
    ui->statusBar->addPermanentWidget(slider);
    ui->statusBar->addPermanentWidget(bar);
    //trying to make the play button
    p->setText("Play");
    //connect(p,SIGNAL (clicked()),player,&QMediaPlayer::play);
    connect(player,&QMediaPlayer::durationChanged,slider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,slider,&QSlider::setValue);
    connect(slider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
    connect(player,&QMediaPlayer::durationChanged,bar,&QProgressBar::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,bar,&QProgressBar::setValue);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open A File","","Video Files (*)");
    on_actionStop_triggered();
    player->setMedia(QUrl::fromLocalFile(filename));
    on_actionPlay_triggered();

}


void MainWindow::on_actionPlay_triggered()
{
    player->play();
    //ui->statusBar->showMessage("Playing");

}


void MainWindow::on_actionStop_triggered()
{
    player->stop();
    //ui->statusBar->showMessage("Stop");
}


void MainWindow::on_actionPause_triggered()
{
    player->pause();
    //ui->statusBar->showMessage("Paused");
}


void MainWindow::on_Pause_clicked()
{
    player->pause();
}




void MainWindow::on_Stop_clicked()
{
    player->stop();
}


void MainWindow::on_Play_clicked()
{
    player->play();
}

