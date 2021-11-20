#include "calculator.h"
#include <QKeyEvent>
#include <QApplication>



Calculator::Calculator(QWidget *parent): QWidget(parent)
{
    createWidgets();
    placeWidget();
    makeConnexions();
}


Calculator::~Calculator()
{
    delete disp;
    delete layout;
    delete buttonsLayout;
    delete left;
    delete right;
    delete operation;

}


void Calculator::createWidgets()
{
    //Creating the layouts
    layout = new QVBoxLayout();
    layout->setSpacing(2);

    //grid layout
    buttonsLayout = new QGridLayout;

    //creating the buttons
    for(int i=0; i < 10; i++)
    {
        digits.push_back(new QPushButton(QString::number(i)));
        digits.back()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        digits.back()->resize(sizeHint().width(), sizeHint().height());
    }
    //enter button
    equal = new QPushButton("=",this);

    //operatiosn buttons
    operations.push_back(new QPushButton("/"));
    operations.push_back(new QPushButton("*"));
    operations.push_back(new QPushButton("-"));
    operations.push_back(new QPushButton("+"));
    operations.push_back(new QPushButton("^"));
    operations.push_back(new QPushButton("%"));

    //unary operations
    unaryOperations.push_back(new QPushButton("Sqrt"));
    unaryOperations.push_back(new QPushButton("x²"));
    unaryOperations.push_back(new QPushButton("1/x"));
    unaryOperations.push_back(new QPushButton("!"));

    //Dot button
    clear = new QPushButton("Clear", this);

    //creating the lineEdit
    disp = new QLineEdit(this);
    disp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Parentheses buttons
    leftParenth = new QPushButton("(", this);
    rightParenth = new QPushButton(")", this);

}

void Calculator::placeWidget()
{

    layout->addWidget(disp);
    layout->addLayout(buttonsLayout);


    //adding the buttons
    for(int i=1; i <10; i++)
        buttonsLayout->addWidget(digits[i], (i-1)/3, (i-1)%3);


    //Adding the operations
    for(int i=0; i < 4; i++)
        buttonsLayout->addWidget(operations[i], i, 4);

    //Add power button
    buttonsLayout->addWidget(operations[4], 0, 6);

    //Add the remainder button
    buttonsLayout->addWidget(operations[5], 1, 6);


    //Adding the unary operations
    for(int i=0; i<4; i++)
        buttonsLayout->addWidget(unaryOperations[i], i, 5);

    //add parentheses
    buttonsLayout->addWidget(leftParenth, 2,6);
    buttonsLayout->addWidget(rightParenth, 3, 6);

    //Adding the 0 button
    buttonsLayout->addWidget(digits[0], 3, 0);

    //Adding the equal button
    buttonsLayout->addWidget(equal, 3, 1);

    //Adding the clear button
    buttonsLayout->addWidget(clear, 3, 2);

    setLayout(layout);
}

void Calculator::makeConnexions()
{
    //connect the digits
    for(int i = 0; i < 10; i++){
        connect(digits[i], &QPushButton::clicked,
                this, &Calculator::newDigit);
    }


    //connect operation buttons
    for(int i=0; i <6; i++){
         connect(operations[i], &QPushButton::clicked,
                 this, &Calculator::changeOperation);
    }

    //connect unaryOperation buttons
    for(int i=0; i <4; i++){
         connect(unaryOperations[i], &QPushButton::clicked,
                 this, &Calculator::unaryOperationSlot);
    }

    //Connect the equal button
        connect(equal, &QPushButton::clicked,
                this, &Calculator::equalButton);

    //Connect the clear button
    connect(clear, &QPushButton::clicked,
            this, &Calculator::clearButton);

    //Connect the left parentheses
    connect(leftParenth, &QPushButton::clicked,
            this, &Calculator::parenthSlot);

    //Connect the right parentheses
    connect(rightParenth, &QPushButton::clicked,
            this, &Calculator::parenthSlot);
}

void Calculator::changeOperation()
{
    //Getting the sender button
    auto button = dynamic_cast<QPushButton*>(sender());

    //Storing the operation
    operation = new QString{button->text()};

    //Initiating the right button
    right = new double{0};

    //reseting the display
    disp->setText(*operation);

    lineEditContent += *operation;
}

void Calculator::newDigit(){

    //Getting the identity of the button using dynamic_cast
    auto button  = dynamic_cast<QPushButton*>(sender());

    // Each button has his own digit in the text
    auto value = button->text().toDouble();
    lineEditContent += QString::number(value);

    if(operation)
        {
            //check if we have a value or not
            if(!right)
                right = new double{value};
            else
                *right = 10 * (*right) + value;

            disp->setText(QString::number(*right));
        }
        else
        {
            if(!left)
                left = new double{value};
            else
                *left = 10 * (*left) + value;

            disp->setText(QString::number(*left));
        }
}

void Calculator::equalButton(){

    if(*operation == "^")
        disp->setText(QString::number(pow(*left, *right)));

    else if(*operation == "%"){
        disp->setText(QString::number(fmod(*left, *right)));
        qDebug() << fmod(*left, *right);
    }
    else {
        QStack<int> operands;
        QStack<QChar> operationStack;

        int pos = 0;
        result = 0;

        while(pos < lineEditContent.size())
        {
              QChar ch = lineEditContent[pos];

              if(ch.isDigit())
              {
                  result = (result * 10) + ch.digitValue();
              }
              else if(Calculator::isOperation(ch))
              {
                    if(ch == '('){
                        operationStack.push(ch);
                        result = 0;
                    }
                    else if(operands.isEmpty()){
                        operands.push(result);
                        operationStack.push(ch);
                        result = 0 ;
                    }
                    else if(ch == ')'){
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
                    }
                    else
                    {
                        QChar prev = operationStack.top();
                        if(Calculator::getPrecedence(ch) > Calculator::getPrecedence(prev)){
                            operands.push(result);
                            operationStack.push(ch);
                            result = 0;
                        }
                        else{
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

        disp->setText(QString::number(result));
    }
}

void Calculator::clearButton(){
    right = nullptr;
    left = nullptr;
    operation = nullptr;
    disp->setText("0");
    lineEditContent = "";
    result = 0;
}

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

void Calculator::keyPressEvent(QKeyEvent *e)
{
    //Exiting the application by a click on space
    if( e->key() == Qt::Key_Escape)
        qApp->exit(0);


    //You could add more keyboard interation here (like digit to button)
}

bool Calculator::isOperation(QChar c){
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
}

double Calculator:: operate(double val1, double val2, QChar op)
{
    if(op == '+') return val1+val2;
    if(op == '-') return val1-val2;
    if(op == '*') return val1*val2;
    return val1/val2;
}

int Calculator::getPrecedence(QChar c){
   if(c == '+' || c == '-') return 1;
   else if(c == '*' || c == '/') return 2;
   else if(c == '(' || c == ')') return 3;
   else return -1;
}

void Calculator::parenthSlot(){

    auto button = dynamic_cast<QPushButton*>(sender());
    auto value = button->text();

    disp->setText(value);

    lineEditContent += value;
}




