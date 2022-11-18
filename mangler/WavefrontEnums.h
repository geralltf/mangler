#ifndef WAVEFRONT_ENUMS_H
#define WAVEFRONT_ENUMS_H

enum OBJToken
{
	WF_UNDEFINED,
	WF_Object,
	WF_Group,
	WF_Vertex,
	WF_VertexParameterSpace,
	WF_VertexTextureCoordinate,
	WF_VertexNormal,
	WF_Face,
	WF_MaterialDefinition, // MTL file defined by: mtllib
	WF_MaterialReference, // Material reference defined by: usemtl
	WF_SmoothShadingParameter,
	WF_CommentLine
};

#endif