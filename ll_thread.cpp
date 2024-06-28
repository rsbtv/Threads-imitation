#include "ll_thread.h"

ll_thread::ll_thread() // конструктор
{
    mtx = new QMutex; // создали мьютекс
    mtx->lock(); // захватили мьютекс
    stch = 0; //
    com = 0; // команды/ответа нет
    ended = false;
}

ll_thread::~ll_thread() // деструктор
{
    qDebug()<<"ll thread уничтожен";
//    com = 2;
//    mtx->unlock(); // освободили мьютекс
//    wait();
    delete mtx;
}


void ll_thread::run() // переопределенный метод потока
{
    do
    {
        mtx->lock(); // попытка захвата мьютекса
//        srand(clock()); // перенастройка генератора псевдослучайных чисел
//        int  r; // для имитации бурной деятельности
        switch (com) // по команде
        {
        case 1: // работать
//            r = rand()%8000;
//            mtx->unlock();
            ended = false;
            break;
        case 2: // пора завершать существование
//            r = rand()%4000;
//            mtx->unlock();
            break;
        default:
//            r = 0;
            break;
        }
//        msleep(r); // работаем или пишем завещание
//        stch = r; // сохраняем результат трудов (время сна)
        msleep(stch);
        switch (com) // формируем ответ
        {
        case 1: // была команда работать
            com = 3;
//            mtx->lock();
            ended = true;
//            mtx->unlock();
            break;
        case 2: // была команда завершать существование
            com = 4;
//            mtx->lock();
            break;
        default:
            com = 0;
            break;
        }
        emit status_upd(); // потоку есть что сообщить
//        mtx->lock();
    }
    while (com != 4); // пока не пришло время умирать
    mtx->unlock();
    qDebug()<<"method run ended";
}
