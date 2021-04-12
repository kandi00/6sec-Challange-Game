#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include <QStringList>
#include <QMessageBox>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct cmpByDistanceTo6Sec {
    bool operator()(const int& a, const int& b) const {
        return abs(6000-a) < abs(6000-b);
    }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int number = 0;
    int counter = 0;
    QTimer* clock;
    QStringList * stringList;
    std::multimap<int, QString, cmpByDistanceTo6Sec> map;

public slots:
    void showTime();

private slots:
    void on_pushButton_clicked();
    void showResults();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
