#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PLPKTree.h"

class PLPKTree : public QMainWindow
{
	Q_OBJECT

public:
	PLPKTree(QWidget *parent = Q_NULLPTR);

private:
	Ui::PLPKTreeClass ui;
};
