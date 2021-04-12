#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <iostream>
#include <QFile>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QTimer - idozitesre
    clock = new QTimer(this);
    clock->start(1000);

    //QString - fileok beolvasasahoz
    stringList = new QStringList("");

    //File tartalmanak beolvasasa a map-be
    QFile file("results.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0,"error", file.errorString());
        return;
    }
    QTextStream in(&file);
    QString line = in.readAll();
    * stringList = QStringList::fromList(line.split('\n'));
    QString elem, nev, ido;
    foreach(elem, *stringList){
        //qDebug() << elem;
        if(elem.isEmpty()) continue;
        QStringList tomb = elem.split(' ');
        ido = tomb[1];
        nev = tomb[2];
        //qDebug() << tomb[0] << " " << ido << " " << nev;
        map.insert({ido.toInt(), nev});
        ++counter;
    }
    file.close();

    connect(clock, SIGNAL(timeout()), this, SLOT(showTime()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(showResults()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTime(){
    ui->lcdNumber->display(number++);
    if(number==7) number = 0;
}

void MainWindow::on_pushButton_clicked()
{
    //the current player's score
    double time;
    if(number == 0){ time = 6000 + this->clock->remainingTime(); }
    else{
        time = (number-1)*1000 + (1000 - (this->clock->remainingTime()));
    }

    //checking if the player will be included in the top 10
    QString nev;
    if(counter < 10){
        nev = QInputDialog::getText(this, "Nev", "Nev");
        map.insert({time, nev});
    }
    else
    {
        //qDebug() << map.end()->first << ' ' << abs(6000-map.end()->first) << ' ' << abs(6000-time);
        if( counter == 10 && (abs(6000-map.end()->first) < abs(6000-time))) {qDebug() << "hhh"; return;}
        else{
            nev = QInputDialog::getText(this, "Nev", "Nev");
            map.erase(prev(map.end()));
            map.insert({time, nev});
        }
    }

    //writing to file the top 10 results
    int helyezes = 1;
    QFile file("results.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::ReadWrite)){
        QTextStream ts(&file);
        for (auto& e : map){
            ts << helyezes++ << ' ' << e.first << ' ' << e.second << Qt::endl;
            if(helyezes == 11) return;
        }
        file.close();
    }

    //    for (auto& e : map){
    //         qDebug() << e.first << e.second << " ";
    //    }
}

void MainWindow::showResults(){

    QFile file("results.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(0,"error", file.errorString());
        return;
    }
    QTextStream in(&file);
    QString line = in.readAll();
    * stringList = QStringList::fromList(line.split('\n'));
    QString elem;
    QString str = "";
    foreach(elem, *stringList){
        str += elem + '\n';
    }
    ui->textEdit->setText(str);
    file.close();

}
