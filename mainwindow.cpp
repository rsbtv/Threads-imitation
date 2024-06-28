#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) // конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    thrd1 = new my_thread(); // создали поток 1
    thrd2 = new my_thread(); // создали поток 2


    srand(time(NULL)); // инициализация датичка псевдослучайных чисел

    ui->pushButton_ll_kill->setEnabled(false);
    ui->pushButton_ll_work->setEnabled(false);
    ui->pushButton_delThread->setEnabled(false);
    ui->textEdit->setReadOnly(false);
}

MainWindow::~MainWindow() // деструктор
{
//    qDebug()<<threads_vect.size();
    delete thrd1; // освободили память класса
    delete thrd2;
    while (true)
    {
        bool done = true;
        for (int i = 0; i < threads_vect.size(); i++)
            if (threads_vect[i]->com != 0 and threads_vect[i]->com != 3)
                done = false;
        if (done)
            break;
            _sleep(100);
    }
    for (int i = 0; i < threads_vect.size(); i++)
    {
        threads_vect[i]->com = 2;
        threads_vect[i]->mtx->unlock();
    }
    for (int i = 0; i < threads_vect.size(); i++)
    {
        threads_vect[i]->wait();
        delete threads_vect[i];
    }
    delete ui;
}


void MainWindow::on_pushButton_Run_clicked() // нажали кнопку запуска
{
    thrd1->stch = rand()%10000; // записываем в стетчик потока случайное число
    // в диапазоне от 0 до 9999 для имитации полезной нагрузки

    thrd2->stch = rand()%10000; // записываем в стетчик потока случайное число
    // в диапазоне от 0 до 9999 для имитации полезной нагрузки
    thrd2->start(); // запускаем поток на выполнение (будет работать переопределенный метод run)
}


void MainWindow::on_pushButton_Show_Stat_clicked() // нажали кнопку вывода статистики
{
    ui->textEdit->append("1: " + ((thrd1->stch > 0)?QString::number(thrd1->stch)+ " msec":"worked") + \
                         " - 2: " + ((thrd2->stch > 0)?QString::number(thrd2->stch) + " msec":"worked"));
}


void MainWindow::on_pushButton_ll_work_clicked() // нажали кнопку команды работать
{
    for (int i = 0; i < threads_vect.size(); i++)
    {
        threads_vect[i]->stch = rand()%8000;
        threads_vect[i]->com = 1;
        threads_vect[i]->mtx->unlock();
        ui->textEdit->append(QString::number(i+1) + " started working");
        counters_vect.append(threads_vect[i]->stch);
    }

    ui->pushButton_ll_kill->setEnabled(false); //отключаем кнопки чтобы пользователь не мешал работе потоков
    ui->pushButton_ll_work->setEnabled(false);
    ui->pushButton_addThread->setEnabled(false);
    ui->pushButton_delThread->setEnabled(false);
}


void MainWindow::on_pushButton_ll_kill_clicked() // нажали кнопку команды умри
{
    ui->pushButton_ll_kill->setVisible(false); // прячем кнопки чтобы ничего кроме лога
    ui->pushButton_ll_work->setVisible(false); // не напоминало о существовании потока
    ui->pushButton_addThread->setVisible(false);
    ui->pushButton_delThread->setVisible(false);
    ui->label->setVisible(false);
    ui->textEdit->append("Why are we still here?");
    for (int i = 0; i < threads_vect.size(); i++)
    {
        threads_vect[i]->stch = rand()%4000;
        threads_vect[i]->com = 2;
        threads_vect[i]->mtx->unlock();
    }
    threads_vect.clear();
}

void MainWindow::potok_info() // слот для приема сигнала от потока ll_thread
{
    ll_thread *thread = qobject_cast<ll_thread*>(sender());
    for (int i = 0; i < threads_vect.size(); i++)
        if (threads_vect[i]->com == 3)
        {
            threads_vect[i]->com = 0;
            ui->textEdit->append(QString::number(i+1) + " stopped after " + QString::number(threads_vect[i]->stch) + " msec hard work");
        }

    auto biggest = std::max_element(std::begin(counters_vect), std::end(counters_vect));
    if (biggest != counters_vect.end())
    {
        int max_stch = std::distance(std::begin(counters_vect), biggest);
        if (threads_vect[max_stch]->ended)
        {
            ui->pushButton_ll_kill->setEnabled(true);
            ui->pushButton_ll_work->setEnabled(true);
            ui->pushButton_addThread->setEnabled(true);
            ui->pushButton_delThread->setEnabled(true);
            counters_vect.clear();

        }
    }
    if (thread->com == 4)
    {
        thread->com = 0;
        ui->textEdit->append(QString::number(thread->number + 1) + " ended after " + QString::number(thread->stch) + " msec crying");
        thread->wait();
        delete thread;
    }
}

void MainWindow::on_pushButton_addThread_clicked() // нажата кнопка добавить поток
{
    ll_thread * llt = new ll_thread();
    llt->com = 0;
    llt->number = threads_vect.size();
    llt->start();
    connect(llt,SIGNAL(status_upd()), this, SLOT(potok_info()));
    threads_vect.append(llt);
    ui->label->setNum(threads_vect.size());
    ui->pushButton_ll_work->setEnabled(true);
    ui->pushButton_ll_kill->setEnabled(true);
    ui->pushButton_delThread->setEnabled(true);
}


void MainWindow::on_pushButton_delThread_clicked() // нажата кнопка удалить поток
{
    if (threads_vect.size() > 0)
    {
        int last_index = threads_vect.size() - 1;
        threads_vect[last_index]->com = 2;
        threads_vect[last_index]->mtx->unlock(); // освободили мьютекс
        threads_vect.pop_back();
        ui->label->setNum(threads_vect.size());

        if (threads_vect.size() == 0)
        {
            ui->pushButton_ll_work->setEnabled(false);
            ui->pushButton_ll_kill->setEnabled(false);
            ui->pushButton_delThread->setEnabled(false);
        }
    }

}

