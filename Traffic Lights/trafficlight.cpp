#include "trafficlight.h"
#include <QWidget>
#include <QLayout>
#include <QRadioButton>
#include<QApplication>

TrafficLight::TrafficLight(QWidget * parent): QWidget(parent){

    //Creatign the widgets
    createWidgets();

    //place Widgets
    placeWidgets();

    startTimer(1000);
}

void TrafficLight::createWidgets()
{

  redlight = new QRadioButton;
  redlight->setEnabled(false);
  redlight->toggle();
  redlight->setStyleSheet("QRadioButton::indicator:checked { background-color: red;}");

  yellowlight = new QRadioButton;
  yellowlight->setEnabled(false);
  yellowlight->setStyleSheet("QRadioButton::indicator:checked { background-color: yellow;}");

  greenlight = new QRadioButton;
  greenlight->setEnabled(false);
  greenlight->setStyleSheet("QRadioButton::indicator:checked { background-color: green;}");

  lights.append(redlight);
  lights.append(redlight);
  lights.append(redlight);
  lights.append(yellowlight);
  lights.append(greenlight);
  lights.append(greenlight);
  index = 0;
  lifeTime =  0;
}


void TrafficLight::placeWidgets()
{

  // Placing the widgets
  auto layout = new QVBoxLayout;
  layout->addWidget(redlight);
  layout->addWidget(yellowlight);
  layout->addWidget(greenlight);
  setLayout(layout);
}

void TrafficLight::timerEvent(QTimerEvent *e)
{

    //augmenter l'index

    index = (index + 1) % 6;

    //Activer le feu Index
    lights[index] -> toggle();




    //Si le feu rouge est activé => activer le jaune
//    if(redlight->isChecked())
//        yellowlight->toggle();
//    //Si le feu jaune est activé => activer le vert
//    else if(yellowlight->isChecked())
//            greenlight->toggle();
//    //Si le feu vert est activé => activer le rouge
//    else
//        redlight->toggle();


//    lifeTime++;

//    //Quand je passe du rouge au jaune
//    if(redlight->isChecked() && lifeTime == 4)
//    {
//        yellowlight->toggle();
//        lifeTime = 0;
//    }
//    else if (yellowlight->isChecked() && lifeTime == 2) {
//        greenlight -> toggle();
//        lifeTime = 0;
//    }
//    else if (greenlight->isChecked() && lifeTime == 1)
//    {
//        redlight->toggle();
//        lifeTime = 0;
//    }


}

void TrafficLight::keyPressEvent(QKeyEvent *e){

    if(e->key() == Qt::Key_Escape)
        qApp->exit();
    else if(e->key() == Qt::Key_R)
        index = 0;
    else if(e->key() == Qt::Key_Y)
        index = 1;
    else if(e->key() == Qt::Key_G)
        index = 2;
    lights[index] -> toggle();
}




