#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include <QObject>

class hello_world : public QObject
{
    Q_OBJECT
public:
    explicit hello_world(QObject *parent = nullptr);
    void sayHello();
signals:
};

#endif // HELLO_WORLD_H
