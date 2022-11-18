#include "MainFrame.h"

MainFrame::MainFrame() : QFrame(nullptr)
{
	setContextMenuPolicy(Qt::ActionsContextMenu);
	QAction* fooAction = new QAction("foo");
	QAction* barAction = new QAction("bar");
	connect(fooAction, SIGNAL(triggered()), this, SLOT(ProvideContexMenu(const QPoint&)));
	connect(barAction, SIGNAL(triggered()), this, SLOT(ProvideContexMenu(const QPoint&)));
	addAction(fooAction);
	addAction(barAction);


	//contextMnu = new QMenu(this);
	//contextMnu->addAction("Add");
	//contextMnu->addAction("Delete");

	//setContextMenuPolicy(Qt::CustomContextMenu);
	//setContextMenuPolicy(Qt::ActionsContextMenu);
	//	connect(ui.tableOptimizationVariables, SIGNAL(customContextMenuRequested(QPoint* position)),frmSettings, SLOT(ProvideContexMenu(position))); 
	//connect(contextMnu, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(ProvideContexMenu(const QPoint&)));
}


#ifndef QT_NO_CONTEXTMENU

void MainFrame::contextMenuEvent(QContextMenuEvent *event)
{
	//QMenu menu(this);
	//menu.addAction("Cut");
	//menu.addAction("Copy");
	//menu.addAction("Paste");
	OpenContextMenu(event->globalPos());
}

void MainFrame::ProvideContexMenu(const QPoint* position)
{
	OpenContextMenu(*position);
}
#endif // QT_NO_CONTEXTMENU