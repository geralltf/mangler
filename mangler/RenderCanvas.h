// (c) Gerallt 2017.

#ifndef RENDER_CANVAS
#define RENDER_CANVAS

#include <fstream>
#include "QSFMLCanvas.h"



class RenderCanvas : public QSFMLCanvas
{

public:

	RenderCanvas(GameConfig& SystemConfig, MainFrame* Parent, const QPoint& Position, const QSize& Size);
	RenderCanvas(GameConfig& SystemConfig, MainFrame* Parent);

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;

	QPaintEngine* paintEngine() override
	{
		return 0;
	}

	void showEvent(QShowEvent*)
	{
		if (!myInitialized)
		{
			// Under X11, we need to flush the commands sent to the server to ensure that
			// SFML will get an updated view of the windows
#ifdef Q_WS_X11
			XFlush(QX11Info::display());
#endif

			// Create the SFML window with the widget handle
			QWidget::create(winId());

			// Let the derived class do its specific stuff
			OnInit();

			// Setup the timer to trigger a refresh at specified framerate
			connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
			myTimer.start();

			myInitialized = true;
		}
	}

	void paintEvent(QPaintEvent*)
	{

		// Let the derived class do its specific stuff
		OnUpdate();

		// Display on screen
		display();
	}
};

#endif