<h1 align="center">Signal and Slots</h1>
<p align="center">
  <img align="center" width="50%"height="50%" src="https://i.ytimg.com/vi/JakMj5XEBoc/maxresdefault.jpg"/>
</p>

## Table of Content
* [About the homework](#about-the-homework)
* [Calculator](#calculator)
  * [Features](#features)
  * [Operators Precedence](#operators-precedence)
  * [Unary Operations](#unary-operations)
  *  [Clear Button](#clear-button)
* [Traffic Lights](#traffic-lights)
  * [First Function](#first-function)
  * [Second Function](#second-function)
  * [Third Function](#third-function)
* [Conclusion](#conclusion)

## About the homework
This homework is the second part of the [Fun With Layouts](https://github.com/Khadija-Bayoud/Fun-With-Layouts) homework, in which we are going to implement an intercative
Calculator and a widget called [Traffic lights](#traffic-lights) that simulate the traffic lights signals.
## Calculator
<p align="center">
  <img align="center" width="60%"height="60%" src="https://user-images.githubusercontent.com/72691265/142721187-1694450a-9861-45a2-89e1-cb6b368738cb.png"/>
</p>

## Features
* Standard Calculator functionality which offers basic operations.
* Support unary and binary operations.
* Evaluating commands using order of operations.

## Operators Precedence
We have rules to indicate which operations take precedence over others, and we often use parentheses to override those rules.
We can remember the order using PEMDAS : Parentheses, Exponents, Multiplication and Division (from left to right). Addition and Substraction (from left to right).

## Algorithm 
Suppose Q is an arithmetic expression.
We use two stacks, one for storing the operands, and the other for storing the operators.
* While there are still tokens to be read in Q,
   * Get the next token.
   * If the token is:
       * A number: push it onto the operands' stack.
       * A left parenthesis: push it onto the operators' stack.
       * A right parenthesis:
         * While the thing on top of the operators' stack is not a 
           left parenthesis,
             * Pop the operator from the operators' stack.
             * Pop the operands' stack twice, getting two operands.
             * Apply the operator to the operands, in the correct order.
             * Push the result onto the operands' stack.
         * Pop the left parenthesis from the operators' stack, and discard it.
       * An operator (thisOp) :
         * While the operators' stack is not empty, and the top thing on the
           operator stack has the same or greater precedence as thisOp,
           * Pop the operator from the operator stack.
           * Pop the value stack twice, getting two operands.
           * Apply the operator to the operands, in the correct order.
           * Push the result onto the operands' stack.
         * Push thisOp onto the operatorss' stack.
* While the operators' stack is not empty,
    * Pop the operator from the operators' stack.
    * Pop the value stack twice, getting two operands.
    * Apply the operator to the operands, in the correct order.
    * Push the result onto the operands' stack.
* At this point the operators' stack should be empty, and the value
   stack should have only one value in it, which is the final result.

Here is the implementation :
```Calculator.cpp```
```cpp
//power and remainder operations
void Calculator::equalButton(){

    if(*operation == "^")
        disp->setText(QString::number(pow(*left, *right)));

    else if(*operation == "%"){
        disp->setText(QString::number(fmod(*left, *right)));
        qDebug() << fmod(*left, *right);
    }
    else {

//Manage operations order
QStack<int> operands;
QStack<QChar> operationStack;

int pos = 0;
result = 0;

while(pos < lineEditContent.size()){
   QChar ch = lineEditContent[pos];
   if(ch.isDigit()){
      result = (result * 10) + ch.digitValue();}
   else if(Calculator::isOperation(ch)){
          if(ch == '('){
               operationStack.push(ch);
               result = 0;
          }else if(operands.isEmpty()){
                operands.push(result);
                operationStack.push(ch);
                result = 0 ;
          }else if(ch == ')'){
                operands.push(result);
                while(operationStack.top() != '('){
                     ch = operationStack.pop();
                     result = operands.pop();
                     int prev = operands.pop();
                     result = Calculator::operate(prev, result, ch);
                      operands.push(result);
                 }
                  operationStack.pop();
                  operands.pop();
          }else{
               QChar prev = operationStack.top();
               if(Calculator::getPrecedence(ch) > Calculator::getPrecedence(prev)){
                  operands.push(result);
                  operationStack.push(ch);
                  result = 0;
              }else{
                  double prevVal = operands.pop();
                  QChar prevOp = operationStack.pop();
                  prevVal = operate(prevVal, result, prevOp);
                  operands.push(prevVal);
                  operationStack.push(ch);
                  result = 0;
              }

         }
              }
    pos++; 
 }

while (!operationStack.isEmpty()) {
     double prev = operands.pop();
     QChar ch = operationStack.pop();
     result = Calculator::operate(prev, result, ch);
}
disp->setText(QString::number(result)); }}
    
```

## Unary Operations
This calculator is capable of performing unary operations such as : square root (sqrt), factorial (!), inverse (1/x), square power (x²) .
Here is the implementation : <br>
```cpp
void Calculator::unaryOperationSlot(){

    //Get the button which send the signal
    auto button = dynamic_cast<QPushButton*>(sender());

    //Get the content of the button
    unaryOperation = new QString{button->text()};

    if(*unaryOperation == "Sqrt"){
        left = new double{sqrt(*left)};
        disp->setText(QString::number(*left));
    }
    else if(*unaryOperation == "x²"){
        left = new double{ (*left) * (*left)};
        disp->setText(QString::number(*left));
    }
    else if(*unaryOperation == "1/x"){
        if(*left != 0){
            left = new double{1/(*left)};
            disp->setText(QString::number(*left));
        }
        else
            disp->setText("Error");
    }
    else{
        for(int i=*left-1; i>0; i--)
            (*left) *= i;
        disp->setText(QString::number(*left));
    }
}

```

## Clear Button
```cpp
void Calculator::clearButton(){
    right = nullptr;
    left = nullptr;
    operation = nullptr;
    disp->setText("0");
    lineEditContent = "";
    result = 0;
}
```

## Traffic Lights
In this exercise, we will use the QTimer to simulate a traffic light.
<p align="center">
  <img align="center" src="https://user-images.githubusercontent.com/72691265/142723271-78604e6d-6c7c-4fd0-8fbf-ef0d9a639682.png"/>
</p>

## First Function
* If the red light is activated => activate yellow light.
* If the yellow light is activated => activate the green light.
* If the green is activated => activate the red light.

Here is the impelementation

```cpp
    if(redlight->isChecked())
        yellowlight->toggle();
    else if(yellowlight->isChecked())
            greenlight->toggle();
    else
        redlight->toggle();
```
## Second Function
* Activate the red light for 4 seconds.
* Activate the yellow light for 2 seconds.
* Activate the green light for 1 seconds.

First implementation :
```cpp
    lifeTime++;
    if(redlight->isChecked() && lifeTime == 4)
    {
        yellowlight->toggle();
        lifeTime = 0;
    }
    else if (yellowlight->isChecked() && lifeTime == 2) {
        greenlight -> toggle();
        lifeTime = 0;
    }
    else if (greenlight->isChecked() && lifeTime == 1)
    {
        redlight->toggle();
        lifeTime = 0;
    }
```

Second implementation :
```cpp
    index = (index + 1) % 6;

    //Activer le feu Index
    lights[index] -> toggle();
```

## Third Function 
Here we use Event Handler.
* If R button is pressed => Activate the red light.
* If Y button is pressend => Activate the yellow light.
* If G button is pressed => Activate the green light.

```cpp
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
```

## Conclusion
*Happy reading!* :heart: :smile: <br>
*Happy coding!* :octocat:






         














