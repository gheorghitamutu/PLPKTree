#include "PLPKTree.h"
#include <QtWidgets/QApplication>
#include "TreeBuilder.h"

int main(int argc, char *argv[])
{
	BuildTree(argv[1]);
	QApplication a(argc, argv);
	PLPKTree w;
	w.show();
	return a.exec();
}
