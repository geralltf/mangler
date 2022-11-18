#include "RenderCanvas.h"

RenderCanvas::RenderCanvas(GameConfig& SystemConfig, MainFrame* Parent, const QPoint& Position, const QSize& Size)
	: QSFMLCanvas(SystemConfig, Parent, Position, Size)
{

}

RenderCanvas::RenderCanvas(GameConfig& SystemConfig, MainFrame* Parent) : QSFMLCanvas(SystemConfig, Parent)
{

}