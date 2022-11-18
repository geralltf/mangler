#include "RenderingContext.h"

RenderingContext::RenderingContext():
	RenderMode(Nullable<::RenderMode>()),
	LineColour(Nullable<ColourRGBA>(ColourRGBA(1.0f, 1.0f, 0.0f, 0.0f))),
	PointColour(Nullable<ColourRGBA>(ColourRGBA(1.0f,0.0f, 1.0f,0.0f))),
	RenderingSize(Vector2i(512, 512)),
HalfSize(Vector2i(256, 256))

{
	Camera = nullptr;
	PointSize = 1;
}

void RenderingContext::DebugLine(Vector3& start, Vector3& end, ColourRGBA& lineColour) const
{
	//Raster.DebugLine(start, end, lineColour);
}

void RenderingContext::DebugPoint(Vector3& start, ColourRGBA& pointColour, int pointSize) const
{
	//Vector4 ? clip;
	//Vector2i s = Camera.Transform.TransformWorldToScreenspace(start, out clip, null).xy;
	//Raster.DrawCircle(s, pointSize, pointColour);
}