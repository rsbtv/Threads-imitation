#include "my_thread.h"

my_thread::my_thread() // конструктор
{
    stch = 0;
}

void my_thread::run() // переопределяемый метод потока
{
    int r = stch;
    stch = -1;
    msleep(r);
    stch = r;
}
// поток ждет случайное времЯ
// в процессе работы в общем поле stch будет -1
// после работы в режиме ожидания в общем поле будет вреия последней работы
// (!) в примере игнорируем требование инкапсуляции (!)
