// #include "mainwindow.h"
// #include <QApplication>

#include <string>
#include <iostream>
#include "textparser.h"

int main(int argc, char *argv[])
{
    TextParser parser{"frankenstein.txt"};
    std::string generated_text = parser.generateText(100);
    std::cout << generated_text << std::endl;
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return QCoreApplication::exec();
}
