#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>
#include <QLineEdit>
#include <cmath>
#include <QStack>
#include <QDebug>

class Calculator : public QWidget
{
    Q_OBJECT
public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();


 // Add your custom slots here
public slots:
    void changeOperation();  //Slot to handle the click on operations
    void newDigit();
    void equalButton();
    void unaryOperationSlot();
    void clearButton();
    void parenthSlot();

protected:
    void createWidgets();        //Function to create the widgets
    void placeWidget();         // Function to place the widgets
    void makeConnexions();      // Create all the connectivity
    bool isOperation(QChar);
    double operate(double, double, QChar);
    int getPrecedence(QChar);
//events
protected:
    void keyPressEvent(QKeyEvent *e)override;     //Override the keypress events

private:
    QGridLayout *buttonsLayout; // layout for the buttons
    QVBoxLayout *layout;        //main layout for the button
    QVector<QPushButton*> digits;  //Vector for the digits
    QPushButton *equal;            // enter button
    QVector<QPushButton*> operations; //operation buttons
    QLineEdit *disp;             // Where to display the numbers
    QVector<QPushButton*> unaryOperations; // Unary operation like sqrt...
    QPushButton *clear;
    QPushButton *leftParenth;
    QPushButton *rightParenth;

//Attributs
private:
    double *left = nullptr;           //left operand
    double *right = nullptr;          // right operand
    QString *operation = nullptr;  // Pointer on the current operation
    QString *unaryOperation = nullptr;
    QString lineEditContent;
    double result;

};
#endif // CALCULATOR_H
