#ifndef WAVEFRONT_MESH_IMPORTER_H
#define WAVEFRONT_MESH_IMPORTER_H

#include <map>
#include "ColourRGBA.h"
#include "WOBJ_Mesh.h"
#include "ParserState.h"
#include "Mesh.h"
#include "FileHelpersh.h"
#include <fstream>
#include <iostream>

const string TOKEN_DELIMITER = " ";
const string TOKEN_INDICIES_DELIMITER = "/";

class WavefrontOBJMeshImporter //TODO: Support mtl files for materials 
{
public:
	int								BufferSize = 128;

	// TEST  per face colours.
	std::map<int, ColourRGBA>*		mapFaceColours = new std::map<int, ColourRGBA>();
	int								faceIDx = 0;

	Mesh* ImportFile(std::string fileName)
	{
		WOBJ_Mesh			meshObj;

		ImportWOBJMesh(fileName, &meshObj);

		Mesh* mesh = new Mesh();

		mesh->GeometryImported = meshObj;

		//mesh.Geometry = HalfEdgeMesh.ProcessWavefrontIntoMesh(mesh.GeometryImported);

		return mesh;
	}

	void ImportWOBJMesh(std::string fileName, WOBJ_Mesh* meshObj)
	{
		ParserState*		state = new ParserState();

		state->file = new ifstream();

		// WOBJ DATA
		state->file->open(fileName, std::ios::in);

		state->mesh = new WOBJ_Mesh();
		state->group->GroupName = "Default";
		state->objName = "";
		state->groupName = "";

		if (state->file->is_open())
		{
			std::cout << "Loading mode " << fileName << std::endl;
			
			while (std::getline(*state->file, state->line))
			{
				// Determine how the line should be parsed.
				state->token = GetToken(state->line);

				handleNewFaceGroup(*state);

				processLineToken(*state, state->token);
			}

			state->file->close();
		}

		delete state->file;

		// Polygon Soup output.
		state->mesh->MeshBuffer->Verticies = state->verticiesList;

		state->mesh->MeshBuffer->FaceColours = state->colours;
		state->mesh->MeshBuffer->Normals = state->normalsList;
		state->mesh->MeshBuffer->NormalIndicies = state->normalIndicies;
		state->mesh->MeshBuffer->TextCoords = state->texCoordList;
		state->mesh->MeshBuffer->TexCoordIndicies = state->texCoordIndicies;

		if (state->mesh->Groups->size() == 0)
		{
			WOBJ_Group* g = new WOBJ_Group();
			g->Indicies = state->indicies;
			g->BeginIndex = 0;
			g->EndIndex = g->Indicies->size() - 1;
			g->GroupName = "Default";
			g->PolygonType = state->polyType;

			state->mesh->Groups->push_back(g);
		}

		meshObj = state->mesh;
	}

	void processLineToken(ParserState& state, OBJToken& token)
	{
		// Parse line.
		switch (token)
		{
		case OBJToken::WF_Object:
			state.objName = ParseName(state.line);

			break;
		case OBJToken::WF_Group:
			state.groupName = ParseName(state.line);

			// Create next group.
			state.group = new WOBJ_Group();
			state.group->GroupName = state.groupName;

			break;
		case OBJToken::WF_Vertex:
			state.vertexCoord = ParseVertexCoord(state.line);

			state.verticiesList->push_back(state.vertexCoord);

			break;
		case OBJToken::WF_VertexParameterSpace:
			//TODO: handle VertexParameterSpace
			break;
		case OBJToken::WF_VertexNormal:
			state.normalCoord = ParseVertexNormal(state.line);

			state.normalsList->push_back(state.normalCoord);

			break;
		case OBJToken::WF_VertexTextureCoordinate:
			state.texCoord = ParseVertexTextureCoordinate(state.line);

			state.texCoordList->push_back(state.texCoord);

			break;
		case OBJToken::WF_Face:
			state.face = ParseFace(state.line);

			// Update min and max coord indexes.
			if (state.face->MinVertIndex < state.minFaceIndex) state.minFaceIndex = state.face->MinVertIndex;
			if (state.face->MaxVertIndex < state.maxFaceIndex) state.maxFaceIndex = state.face->MaxVertIndex;

			// Determine polygon type.
			if (state.face->VertexCount == 3) state.polyType = PolygonType::PT_Triangles;
			if (state.face->VertexCount == 4) state.polyType = PolygonType::PT_Quads;

			state.indicies->insert(state.indicies->end(), state.face->VertexCoordIndicies->begin(), state.face->VertexCoordIndicies->end()); //state.indicies.AddRange(state.face.VertexCoordIndicies);
			state.normalIndicies->insert(state.normalIndicies->end(), state.face->VertexNormalIndicies->begin(), state.face->VertexNormalIndicies->end()); //state.normalIndicies.AddRange(state.face->VertexNormalIndicies);\
			state.texCoordIndicies->insert(state.texCoordIndicies->end(), state.face->VertexTexCoordIndicies->begin(), state.face->VertexTexCoordIndicies->end()); //state.texCoordIndicies.AddRange(state.face->VertexTexCoordIndicies);

			ColourRGBA colour = Colour::Randomise();
			state.colours->push_back(&colour);

			++faceIDx;

			break;
		case OBJToken::WF_MaterialDefinition:
			state.mtlFile = ParseMTLLibLocation(state.line);
			//TODO: parse .mtl files
			break;
		case OBJToken::WF_MaterialReference:
			state.materialRef = ParseMaterialRef(state.line);
			//TODO: handle materials
			break;
		case OBJToken::WF_SmoothShadingParameter:
			//TODO: implement smooth shading
			state.numSmoothingGroups = ParseSmoothingGroups(state.line);
			break;
		case OBJToken::WF_CommentLine:
			// Ignore.
			break;
		case OBJToken::WF_UNDEFINED:
			// TODO: Issue a syntax error and reference line number
			break;
		}
	}

	static void handleNewFaceGroup(ParserState& state)
	{
		// Handle different polygon types by parsing each face set group.
		if (state.token == OBJToken::WF_Face)
		{
			if (state.atFace == false)
			{
				// Begin new face set
				state.minFaceIndex = INT32_MAX;
				state.maxFaceIndex = INT32_MIN;
				state.polyType = Nullable<PolygonType>();
			}
			state.atFace = true;
		}
		else
		{
			if (state.atFace == true)
			{
				if (state.group != nullptr)
				{
					state.group->Indicies = state.indicies;
					state.group->BeginIndex = state.minFaceIndex == INT32_MAX ? -1 : state.minFaceIndex;
					state.group->EndIndex = state.maxFaceIndex == INT32_MIN ? -1 : state.maxFaceIndex;

					if (state.polyType.is_set()) state.group->PolygonType = state.polyType.get();

					// Add previous group (if any) to list.
					state.mesh->Groups->push_back(state.group);

					state.indicies = new std::vector<int*>();
				}
			}

			state.atFace = false;
		}
	}

	OBJToken& GetToken(std::string line)
	{
		OBJToken token = OBJToken::WF_UNDEFINED;

		if (startswith(line, "v "))
		{
			token = OBJToken::WF_Vertex;
		}
		else if (startswith(line, "vt "))
		{
			token = OBJToken::WF_VertexTextureCoordinate;
		}
		else if (startswith(line, "vn "))
		{
			token = OBJToken::WF_VertexNormal;
		}
		else if (startswith(line, "vp "))
		{
			token = OBJToken::WF_VertexParameterSpace;
		}
		else if (startswith(line, "f "))
		{
			token = OBJToken::WF_Face;
		}
		else if (startswith(line, "o "))
		{
			token = OBJToken::WF_Object;
		}
		else if (startswith(line, "g "))
		{
			token = OBJToken::WF_Group;
		}
		else if (startswith(line, "mtllib"))
		{
			token = OBJToken::WF_MaterialDefinition;
		}
		else if (startswith(line, "usemtl"))
		{
			token = OBJToken::WF_MaterialReference;
		}
		else if (startswith(line, "s "))
		{
			token = OBJToken::WF_SmoothShadingParameter;
		}
		else if (startswith(line, "#"))
		{
			token = OBJToken::WF_CommentLine;
		}

		return token;
	}

	int ParseSmoothingGroups(std::string line)
	{
		std::string s = line;
		int numberOfSmoothingGroups;

		trim(s);
		if(startswith(s, "s "))
		{
			remove(s, 0, 1);
		}
		ltrim(s);

		if (int_try_parse(s, numberOfSmoothingGroups))
		{

		}
		else
		{
			if (s == "off")
			{
				// TODO: set smoothing off
			}
		}


		return numberOfSmoothingGroups;
	}

	std::string& ParseMTLLibLocation(const std::string& line) const
	{
		string mtllib = line;
		ltrim(mtllib);
		if(startswith(mtllib, "mtllib"))
		{
			remove(mtllib, 0, 6); // remove the "mtllib" token.
		}
		trim(mtllib);
		return mtllib;
	}

	std::string& ParseMaterialRef(std::string& line)
	{
		std::string usemtl = line;
		ltrim(usemtl);
		if(startswith(usemtl, "usemtl"))
		{
			remove(usemtl, 0, 6); // remove the "usemtl" token.
		}
		trim(usemtl);
		return usemtl;
	}

	std::string ParseName(std::string line)
	{
		// o Sphere
		// g [group name]
		std::string objName = line;
		ltrim(objName);
		if(startswith(objName, "o ") || startswith(objName, "g "))
		{
			remove(objName, 0, 1);
			ltrim(objName);
		}
		trim(objName);
		return objName;
	}

	bool try_parse_vector3(std::string& line, Nullable<Vector3>& outVector)
	{
		std::vector<string*>* coordsList = nullptr;

		split(line, TOKEN_DELIMITER, coordsList, true);
		//string[] coords = vert.Split(' ').Where(s = > !string.IsNullOrEmpty(s.Trim())).ToArray();

		float x;
		bool has_x = float_try_parse(*coordsList->at(0), x);

		float y;
		bool has_y = float_try_parse(*coordsList->at(1), y);

		float z;
		bool has_z = float_try_parse(*coordsList->at(2), z);

		if(has_x && has_y && has_z)
		{
			outVector = Vector3(x, y, z);
			return true;
		}
		else
		{
			outVector = Nullable<Vector3>();
		}
		return false;
	}

	bool try_parse_vector2(std::string& line, Nullable<Vector2>& outVector)
	{
		std::vector<string*>* coordsList = nullptr;

		split(line, TOKEN_DELIMITER, coordsList, true);

		float x;
		bool has_x = float_try_parse(*coordsList->at(0), x);

		float y;
		bool has_y = float_try_parse(*coordsList->at(1), y);

		if (has_x && has_y)
		{
			outVector = Vector2(x, y);
			return true;
		}
		else
		{
			outVector = Nullable<Vector2>();
		}
		return false;
	}

	Vector3* ParseVertexCoord(std::string& line)
	{
		Vector3 out;
		Nullable<Vector3> v;

		// v -1.380437 -0.624475 0.886924
		std::string vert = line;
		ltrim(vert);
		if (startswith(vert, "v "))
		{
			remove(vert, 0, 1);
			ltrim(vert);
		}

		if(try_parse_vector3(vert, v))
		{
			out = v.get();
		}
		else
		{
			out = Vector3::Zero;
		}
		
		return &out;
	}

	Vector2* ParseVertexTextureCoordinate(std::string& line)
	{
		Vector2 out;
		Nullable<Vector2> v;

		// vt 0.920424 0.491081
		std::string tex = line;
		ltrim(tex);
		if (startswith(tex, "vt "))
		{
			remove(tex, 0, 2);
			ltrim(tex);
		}

		if(try_parse_vector2(tex, v))
		{
			out = v.get();
		}
		else
		{
			out = Vector2::Zero;
		}

		return &out;
	}

	Vector3* ParseVertexNormal(std::string& line)
	{
		Vector3 out;
		Nullable<Vector3> v;

		// vn -0.464500 -0.402700 -0.788700
		string norm = line;
		ltrim(norm);
		if(startswith(norm, "vn "))
		{
			remove(norm, 0, 2);
			ltrim(norm);
		}
		
		if (try_parse_vector3(norm, v))
		{
			out = v.get();
		}
		else
		{
			out = Vector3::Zero;
		}

		return &out;
	}

	WOBJ_Face* ParseFace(string line)
	{
		WOBJ_Face* face = new WOBJ_Face();
		// Valid formats are the following: 
		// 1). Vertex, normal, tex:                f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
		// 2). Vertex and Tex:                     f v1/vt1 v2/vt2 v3/vt3 ...
		// 3). Vertex indicies only:               f v1 v2 v3 ....
		// 4). Vertex, and normal without tex:     f v1//vn1 v2//vn2 v3//vn3 ...

		/* Samples:
		f 606/962/1686 434/880/1686 455/882/1686
		f 876/1020/1687 561/435/1687 315/1074/1687
		f 951/1266/1688 863/703/1688 395/670/1688
		f 555/1181/1689 876/1020/1689 315/1074/1689
		f 457/861/1690 435/606/1690 823/147/1690
		f 394/473/1691 240/472/1691 25/475/1691
		f 713/245/1692 988/244/1692 364/420/1692
		f 809/267/1693 326/759/1693 462/871/1693
		f 594/1192/1694 228/185/1694 327/1035/1694
		f 185/1039/1695 605/925/714 186/963/714
		f 506/456/1696 957/710/1696 232/595/1696
		f 668/582/1697 348/612/1697 961/1217/1697
		f 475/102/1698 369/104/1698 206/552/1698
		*/

		string f = line;
		std::vector<string*>* verts = nullptr;
		int vertInd;
		std::string vert;
		std::vector<string*>* indicies = nullptr;

		ltrim(f);
		if(startswith(f, "f "))
		{
			remove(f, 0, 1);
			ltrim(f);
		}
		trim(f);
		
		split(f, TOKEN_DELIMITER, verts);

		face->MinVertIndex = -1;
		face->MaxVertIndex = -1;

		if (verts->size() == 0)
		{
			face = nullptr; // Skip empty faces
		}
		else
		{
			face->MinVertIndex = INT32_MAX;
			face->MaxVertIndex = INT32_MAX;
			face->VertexCount = verts->size();

			vertInd = 0;
			for(auto it = verts->begin(); it < verts->end(); ++it)
			{
				vert = **it; // A vertex with a bunch of coordinate indexes.

				split(vert, TOKEN_INDICIES_DELIMITER, indicies); // An array of indicies for that vertex.

				if(nullorempty(trim_copy(*indicies->at(0))) || indicies->size() == 0)
				{
					// TODO: issue syntax error since there should at minimal be a vertex index (coordIndex) on this line number.

					continue;
				}
				else
				{
					int coordIndex;
					std::string s_coord = trim_copy(*indicies->at(0));
					if(int_try_parse(s_coord, coordIndex))
					{
						//int coordIndex = int.Parse(indicies[0].Trim()) - 1; // Index is not zero based
						int texCoordIndex;
						int normCoordIndex;

						if (coordIndex < face->MinVertIndex)
							face->MinVertIndex = coordIndex;
						if (coordIndex > face->MaxVertIndex)
							face->MaxVertIndex = coordIndex;

						face->VertexCoordIndicies->push_back(coordIndex);

						if (indicies->size() > 1)
						{
							if (nullorempty(*indicies->at(1))) // Texture Coord Index
							{
								// Ignore.
								face->HasTextureCoords = false;
							}
							else
							{
								face->HasTextureCoords = true;

								if (int_try_parse(*indicies->at(1), texCoordIndex))
								{
									texCoordIndex = texCoordIndex - 1; // Index is not zero based

									face->VertexTexCoordIndicies->push_back(texCoordIndex);
								}
								else
								{
									// TODO: Problem parsing the index. Issue syntax error.
								}
							}
						}

						if (indicies->size() > 2)
						{

							if (nullorempty(*indicies->at(2))) // Normal coord index.
							{
								// Ignore.
								face->HasNormals = false;
							}
							else
							{
								face->HasNormals = true;

								if (int_try_parse(*indicies->at(2), normCoordIndex))
								{
									normCoordIndex = normCoordIndex - 1; // Index is not zero based

									face->VertexNormalIndicies->push_back(normCoordIndex);
								}
								else
								{
									// TODO:  Problem parsing the index. Issue syntax error.
								}

								//normCoordIndex = int.Parse(indicies[2].Trim()) - 1; // Index is not zero based
							}
						}
					}
					else
					{
						// TODO: Problem parsing the coordinate index. Issue syntax error.
						continue;
					}
				}

				++vertInd;
			}

			//for (vertInd = 0; vertInd < verts->size(); vertInd++)
			//{
				//vert = verts[vertInd]; // A vertex with a bunch of coordinate indexes.
				//indicies = vert.Split('/'); // An array of indicies for that vertex.

				//if (string.IsNullOrEmpty(indicies[0].Trim()) || indicies.Length == 0)
				//{
				//	// TODO: issue syntax error since there should at minimal be a vertex index (coordIndex) on this line number.

				//	continue;
				//}
				//else
				//{
				//	int coordIndex = int_try_parse();
				//	//int coordIndex = int.Parse(indicies[0].Trim()) - 1; // Index is not zero based
				//	int texCoordIndex;
				//	int normCoordIndex;

				//	if (coordIndex < face.MinVertIndex)
				//		face.MinVertIndex = coordIndex;
				//	if (coordIndex > face.MaxVertIndex)
				//		face.MaxVertIndex = coordIndex;

				//	face.VertexCoordIndicies.Add(coordIndex);

				//	if (indicies->size() > 1)
				//	{
				//		if (string.IsNullOrEmpty(indicies[1])) // Texture Coord Index
				//		{
				//			// Ignore.
				//			face.HasTextureCoords = false;
				//		}
				//		else
				//		{
				//			face.HasTextureCoords = true;

				//			//int_try_parse(indicies->at(1), )
				//			texCoordIndex = int.Parse(indicies[1].Trim()) - 1; // Index is not zero based

				//			face.VertexTexCoordIndicies.Add(texCoordIndex);
				//		}
				//	}

				//	if (indicies.Length > 2)
				//	{
				//		if (string.IsNullOrEmpty(indicies[2]))
				//		{
				//			// Ignore.
				//			face.HasNormals = false;
				//		}
				//		else
				//		{
				//			face.HasNormals = true;

				//			normCoordIndex = int.Parse(indicies[2].Trim()) - 1; // Index is not zero based

				//			face.VertexNormalIndicies.Add(normCoordIndex);
				//		}
				//	}
				//}
			//}
		}

		return face;
	}
};

#endif