#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ManglerSource.h"

class ManglerSource : public QMainWindow
{
	Q_OBJECT

public:
	ManglerSource(QWidget *parent = Q_NULLPTR);

private:
	Ui::ManglerSourceClass ui;
};
