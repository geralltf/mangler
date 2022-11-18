#ifndef MESH_H
#define MESH_H

class Transform;
class Mesh;

#include "Nullable.h"
#include "Transform.h"
#include "HalfEdgeMesh.h"
#include "WOBJ_Mesh.h"

class Mesh
{
public:
	Transform* transform;
	Transform* parent;
	std::vector<Mesh*>* ChildrenMeshes;

	Nullable<PolygonType> PolygonType;
	HalfEdgeMesh Geometry; // the actual geometry to transform
	WOBJ_Mesh GeometryImported;

	Mesh()
	{
		parent = nullptr;
		transform = new Transform();
		ChildrenMeshes = new std::vector<Mesh*>();
	}

	Mesh(std::vector<Mesh*>* children)
	{
		parent = nullptr;
		ChildrenMeshes = children;
	}

	void AddChild(Mesh* childMesh)
	{
		ChildrenMeshes->push_back(childMesh);
	}
};

#endif