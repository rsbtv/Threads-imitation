#ifndef LL_THREAD_H
#define LL_THREAD_H

#include <QThread>
#include <QMutex> //подключаем класс мьютексов
#include <QDebug>


class ll_thread : public QThread
{
    Q_OBJECT // добавляем для работы с сигналами (после добавления надо удалить билд)

public:
    ll_thread();
    ~ll_thread();

    void run();

    QMutex * mtx; // указатель на мьютекс
    int stch; // счетчик
    int com; // код команды/ответа (1 - работать, 2 - умри
             //                     3 - закончил работать, 4 - прощай дивный мир)
    int number;

    bool ended;

signals:
    void status_upd(); // сигнал изменения статуса (состаяния) потока
};

#endif // LL_THREAD_H
