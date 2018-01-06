#include <QApplication>
#include <QTime>
#include <QMainWindow>

#include "graphwidget.h"
#include "InputFile.h"

int main(int argc, char *argv[])
{
	InputFile* input_file = new InputFile;
	input_file->ReadFile(argv[1]);
	input_file->ParseInput();
	input_file->ParseTree();
	std::vector<std::vector<Expression*>> tree = input_file->GetTree();
	delete input_file;

	QApplication app(argc, argv);

	GraphWidget *widget = new GraphWidget;
	widget->SetTree(tree);

	QMainWindow mainWindow;
	mainWindow.setCentralWidget(widget);

	mainWindow.show();

	return app.exec();
}
