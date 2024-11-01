#include "hello_world.h"
#include "QDebug"

hello_world::hello_world(QObject *parent)
    : QObject{parent}
{}

void hello_world::sayHello(){
    qDebug() << "Hello,world.";
}
