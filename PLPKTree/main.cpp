#include "PLPKTree.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PLPKTree w;
	w.show();
	return a.exec();
}
