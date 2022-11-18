#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

class Camera;

#include "Nullable.h"
#include "Enums.h"
#include "ColourRGBA.h"
#include "Camera.h"
#include "Vector2i.h"
#include "Vector3.h"

class RenderingContext
{
public:
	Nullable<RenderMode>& RenderMode;
	Nullable<ColourRGBA>& LineColour;
	Nullable<ColourRGBA>& PointColour;
	int PointSize = 1;
	
	bool IgnoreCameraTranslation = false;
	Vector2i& RenderingSize;
	Vector2i& HalfSize;

	Camera* Camera;

	RenderingContext();

	void DebugLine(Vector3& start, Vector3& end, ColourRGBA& lineColour) const;
	void DebugPoint(Vector3& start, ColourRGBA& pointColour, int pointSize) const;
};

#endif