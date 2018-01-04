#include "PLPKTree.h"
#include <QtWidgets/QApplication>
#include "InputFile.h"

int main(int argc, char *argv[])
{
	InputFile input_file;
	input_file.ReadFile(argv[1]);
	input_file.ParseInput();
	input_file.ParseTree();

	QApplication a(argc, argv);
	PLPKTree w;
	w.show();
	return a.exec();
}
