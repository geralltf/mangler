// (c) Gerallt 2017.

#include "QSFMLCanvas.h"
#include "GameConfig.h"
#include <iostream>

#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif

#define IS_EDITOR_WINDOW() RenderWindow((sf::WindowHandle)winId()) 
#define IS_GAME_WINDOW() RenderWindow(SystemConfig.GetVideoMode(), SystemConfig.GetAppTitle(), SystemConfig.GetDisplayStyle())

static void loadFile()
{
	std::cout << "aaaaaa " << std::endl;
}
static void saveFile()
{

}
bool QSFMLCanvas::eventFilter(QObject *object, QEvent *event)
{
	if (object == this && event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

		if (keyEvent->key() == Qt::Key_Tab) {
			return true;
		}
		else
			return false;
	}
	return false;
}
void QSFMLCanvas::LoadFile() {
	auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

}

QSFMLCanvas::QSFMLCanvas(GameConfig& SystemConfig, MainFrame* Parent, const QPoint& Position, const QSize& Size)
	: QWidget(Parent), myInitialized(false), IS_EDITOR_WINDOW()
{
	this->SystemConfig = &SystemConfig;
	this->parentFrame = Parent;

	// Setup some states to allow direct rendering into the widget
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);

	// Set strong focus to enable keyboard events to be received
	setFocusPolicy(Qt::StrongFocus);

	// dont show default Qt widget, it is unnessesary and only decreases sfml framerate performance.
	setAttribute(Qt::WA_DontShowOnScreen);
	installEventFilter(this);

	//setFocus();

	// Setup the widget geometry
	move(Position);
	resize(Size);


	// Menu system

	QVBoxLayout *boxLayout = new QVBoxLayout(Parent); // Main layout of widget

	QAction *saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the current scene"));
	connect(saveAct, &QAction::triggered, this, &saveFile);

	QAction *loadAct = new QAction(tr("&Load"), this);
	loadAct->setShortcuts(QKeySequence::Open);
	loadAct->setStatusTip(tr("Load an existing scene from file"));
	connect(loadAct, &QAction::triggered, this, &loadFile);
	connect(loadAct, SIGNAL(triggered()), this, SLOT(LoadFile()));

	QMenuBar* menuBar = new QMenuBar();
	QMenu *fileMenu = new QMenu("File");
	menuBar->addMenu(fileMenu);
	fileMenu->addAction(loadAct);
	fileMenu->addAction(saveAct);
	fileMenu->addSeparator();
	fileMenu->addAction("Exit");

	fileMenu->connect(loadAct, SIGNAL(triggered()), fileMenu, SLOT(LoadFile()));

	Parent->layout()->setMenuBar(menuBar);
	//QToolBar *fileToolBar = addToolBar(tr("File"));

	//const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));

	//QAction *newAct = new QAction(newIcon, tr("&New"), this);
	//newAct->setShortcuts(QKeySequence::New);
	//newAct->setStatusTip(tr("Create a new file"));
	//connect(newAct, &QAction::triggered, this, &newFile);
	//fileMenu->addAction(newAct);
	//fileToolBar->addAction(newAct);
}

QSFMLCanvas::QSFMLCanvas(GameConfig& SystemConfig, MainFrame* Parent)
	: QWidget(Parent), myInitialized(false), IS_GAME_WINDOW()

{
	auto pconf = &SystemConfig;
	this->SystemConfig = pconf;

	// break out of Qt window 
	Parent->setVisible(false);
	Parent->hide();
	Parent->showMinimized();
	setAttribute(Qt::WA_DontShowOnScreen);
}

QSFMLCanvas::~QSFMLCanvas()
{

}

void QSFMLCanvas::showEvent(QShowEvent*)
{
	if (!myInitialized)
	{
		// Under X11, we need to flush the commands sent to the server to ensure that
		// SFML will get an updated view of the windows
#ifdef Q_WS_X11
		XFlush(QX11Info::display());
#endif

		// Create the SFML window with the widget handle
		//QWidget::create(winId()); // didn't work, so set the QWidget as the owner in ctor using: RenderWindow((sf::WindowHandle)winId())



		// Let the derived class do its specific stuff
		OnInit();


		if (SystemConfig->IsEnabled(GameStartupOption::CAP_FPS))
		{
			framerateFreq = SystemConfig->GetFloat("display", "fcapframeratefreg");

			std::cout << "[framerateFreq " << framerateFreq << "]" << std::endl;
		}

		// Setup the timer
		myTimer.setInterval(framerateFreq);


		// Setup the timer to trigger a refresh at specified framerate
		connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
		myTimer.start();



		myInitialized = true;
	}
}



QPaintEngine* QSFMLCanvas::paintEngine()
{
	return nullptr;
}


void QSFMLCanvas::paintEvent(QPaintEvent*)
{
	// Let the derived class do its specific stuff
	OnUpdate();

	// Display on screen
	display();
}


