#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVector>
#include "my_thread.h"
#include "ll_thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    my_thread * thrd1; // указатель на поток 1
    my_thread * thrd2; // указатель на поток 2

//    ll_thread * llt; // указатель
//    ll_thread * llt2;
//    ll_thread * llt3;
    QVector<ll_thread*>  threads_vect;
    QVector<int> counters_vect;
    int global_val;
    int pressed = 0;


public slots:
    void potok_info(void); // слот для приема сигналов от потока ll_thread

private slots:
    void on_pushButton_Run_clicked();

    void on_pushButton_Show_Stat_clicked();

    void on_pushButton_ll_work_clicked();

    void on_pushButton_ll_kill_clicked();

//    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_addThread_clicked();

    void on_pushButton_delThread_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
