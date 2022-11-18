#ifndef MAIN_FRAME
#define MAIN_FRAME

#include <QFrame>
#include <QMenu>
#include <QtWidgets>
#include <qwidget.h>
#include <SFML/Graphics.hpp>

class MainFrame : public QFrame
{
	Q_OBJECT
public:
	QMenu*			contextMnu;
	MainFrame();

	public slots:
	void ProvideContexMenu(const QPoint* position);

public:

	void OpenContextMenu(QPoint position) const
	{
		contextMnu->exec(position);
	}

	void OpenContextMenu(sf::Vector2f position) const
	{
		QPoint pos(position.x, position.y);
		contextMnu->exec(pos);
	}

#ifndef QT_NO_CONTEXTMENU
	void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
};

#endif