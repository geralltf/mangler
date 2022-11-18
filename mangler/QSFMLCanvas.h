// (c) Gerallt 2017.

#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <SFML/Graphics.hpp>
#include <QtWidgets>
#include <qwidget.h>
#include <qtimer.h>
#include "GameConfig.h"
#include "MainFrame.h"
#include <QFileDialog>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
	//Q_OBJECT
protected:
	QTimer			myTimer;
	bool			myInitialized;
	GameConfig*		SystemConfig;
	unsigned int	framerateFreq = 16; // 60hz or 1/60
	MainFrame*		parentFrame;
	public slots:
	void LoadFile();

public:

	QSFMLCanvas(GameConfig& SystemConfig, MainFrame* Parent, const QPoint& Position, const QSize& Size);
	QSFMLCanvas(GameConfig& SystemConfig, MainFrame* Parent);

	bool eventFilter(QObject *object, QEvent *event) override;

	void OpenContextMenu(sf::Vector2f position) const {
		parentFrame->OpenContextMenu(position);
	}
	void OpenContextMenu(QPoint position) const {
		parentFrame->OpenContextMenu(position);
	}

	virtual ~QSFMLCanvas();

private:

	// SFML
	virtual void OnInit() = 0;

	virtual void OnUpdate() = 0;

	// Qt
	virtual QPaintEngine* paintEngine() = 0;

	virtual void showEvent(QShowEvent*) = 0;

	virtual void paintEvent(QPaintEvent*) = 0;

};


#endif // QSFMLCANVAS_H
