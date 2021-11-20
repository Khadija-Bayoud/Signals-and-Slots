#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <QWidget>
#include<QKeyEvent>
#include<QTimerEvent>
#include<QVector>
class QRadioButton;

class TrafficLight: public QWidget{
  Q_OBJECT

public:

  TrafficLight(QWidget * parent = nullptr);

protected:
     void createWidgets();
     void placeWidgets();
     void timerEvent(QTimerEvent *e) override;
     void keyPressEvent(QKeyEvent* e) override;

private:

  QRadioButton * redlight;
  QRadioButton * yellowlight;
  QRadioButton * greenlight;

  QVector<QRadioButton*> lights;
  int index; //indice du feu active

  int lifeTime; //La durée de vie du feu

};


#endif
