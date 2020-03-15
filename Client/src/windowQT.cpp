#include "../includes/windowQT.hpp"

WindowQT::WindowQT(int id, MainWindow *parent, Client* client):
QWidget(parent),
parent(parent),
id(id),
intervalle(0),
client(client),
timer(nullptr)
{
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), parent, SLOT(setPage(int)));
}

int WindowQT::getId(){
    return id;
}

void WindowQT::startTimer(){
  if (timer){
    timer->start(intervalle);
  }
}

void WindowQT::stopTimer(){
  if (timer){
    timer->stop();
  }
}

void WindowQT::setTimerIntervalle(int t){
  intervalle = t;
}
