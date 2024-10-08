#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Timer = new QTimer();
    connect(Timer, SIGNAL(timeout()), this, SLOT(Capture_Screen_Image()));
    Timer->setInterval(300);

    webserver.start();

    for (int i = 0; i < QGuiApplication::screens().size() ; i++) {
        qDebug() << QGuiApplication::screens().at(i)->name();
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap ScreenImage = screen->grabWindow(0);
    ScreenImage.save(QCoreApplication::applicationDirPath() + "/ScreenImage.bmp");
    ui->label->setPixmap(ScreenImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CaptureBtn_clicked()
{
    Timer->stop();
}

void MainWindow::Capture_Screen_Image()
{
    ImageCounter = 1;
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap image = screen->grabWindow(0);
    image.save("/home/kunal/workspace/Qt Practice/QScreenApp/Screen Image/ScreenImage_"+QString::number(ImageCounter)+".PNG");
    ui->label->setPixmap(image);
//    qDebug() << QCoreApplication::applicationDirPath();
}


void MainWindow::on_CaptureStartBtn_clicked()
{
    Timer->start();
}

