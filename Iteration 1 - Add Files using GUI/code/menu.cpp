#include <QMenu>
#include <QMenuBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <iostream>
#include "menu.h"
#include <iostream>
#include <cstddef>
#include <string>
#include <vector>

TheMenu::TheMenu(ThePlayer *player,std::vector<TheButton*> buttons,std::vector<TheButtonInfo> videos,QHBoxLayout *layout,QWidget *parent)
    : QMainWindow(parent),player(player),buttons(buttons),videos(videos),layout(layout),buttonWidget(parent) {

//  QPixmap openpix("openfolder.jpeg");
//  QPixmap quitpix("quit.png");
//  player=player;
  auto *openFile =  new QAction("&Open File", this);
  auto *openFolder =  new QAction("&Open Folder", this);
  auto *quit = new QAction( "&Quit", this);
  quit->setShortcut(tr("CTRL+Q"));

  QMenu *file = menuBar()->addMenu("&File");
    file->addAction(openFile);
    file->addAction(openFolder);
    file->addSeparator();
    file->addAction(quit);
  qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
  connect(openFile, &QAction::triggered, this, &TheMenu::openFile);
  connect(openFolder, &QAction::triggered, this, &TheMenu::openFolder);
  connect(quit, &QAction::triggered, qApp, QApplication::quit);
}

std::vector<TheButtonInfo> TheMenu::getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out ;
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
            std::cout<<"found mp4 videos"<<std::endl;
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

void TheMenu::openFolder()
{




    //taking video url
    QString filename=QFileDialog::getExistingDirectory(this,"Open a Folder", "/home",
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);
    std::cout<<filename.toStdString()<<std::endl;
    if(!filename.isEmpty()){
        videos=getInfoIn(filename.toStdString());

        // a list of the buttons
        std::vector<TheButton*> Buttons;

        std::cout<<"Creating buttons "<<std::endl;
        // create the buttons
        for ( size_t i = 0; i < videos.size(); i++ ) {
            TheButton *button = new TheButton(buttonWidget);
            button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
            Buttons.push_back(button);
            layout->addWidget(button);
            button->init(&videos.at(i));
        }

        std::cout<<"calling set content "<<std::endl;

    //    // Menu Widget
    //    TheMenu *menu=new TheMenu(player,buttons,videos,buttonWidget,);

        // tell the player what buttons and videos are available
        player->setContent(&Buttons, &videos);
        buttons=Buttons;



}
}


void TheMenu::openFile()
{

//        QString fileName = QFileDialog::getExistingDirectory(this);
//        if (!fileName.isEmpty())
//            loadFile(fileName);

    //taking video url
    QString filename=QFileDialog::getOpenFileName(this,"Open a File","","Video File (*.*)");
    std::cout<<filename.toStdString()<<std::endl;
    if(!filename.isEmpty()){
        TheButton *button = new TheButton(buttonWidget);


      //taking video icon(demo)

        // std::string str="/home/rosh/NITRBrahs/qt-video/videos/demo.png";
        // QString thumb = QString::fromStdString(str);
        // QImageReader *imageReader = new QImageReader(thumb);
        // QImage sprite = imageReader->read();
        // QIcon* ico = new QIcon(QPixmap::fromImage(sprite));
        QString thumb = filename.left(filename.length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an  
                        QUrl* url = new QUrl(QUrl::fromLocalFile(filename));

                        TheButtonInfo* bi= new TheButtonInfo(url,ico);

                        button->connect(button, SIGNAL(jumpTo(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));

                        //add to layout
                        videos.push_back(*bi);
                        button->init(bi);
                        buttons.push_back(button);
                        layout->addWidget(button);

                    }     
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;            

    //updating setting playlist again
     player->setContent(&buttons, &videos);

    //playing video
      player->playVideo(filename);
    }
   else
     qDebug() << "warning: cannot add empty video " << endl;


}
