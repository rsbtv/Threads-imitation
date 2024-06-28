#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <QThread> // Подключаем базовый класс потоков Qt


class my_thread : public QThread // добавляем наследование от базового класса
{
public:
    my_thread();
    void run(); // для переопределяемой функции потока
    int stch; // для имитации бурной деятельности
};

#endif // MY_THREAD_H
