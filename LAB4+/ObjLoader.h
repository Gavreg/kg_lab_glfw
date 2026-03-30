#pragma once


#include <list>
#include <GLFW/glfw3.h>

struct ObjVertex
{
	double x = 0, y = 0, z = 0, w = 1;

	inline double* _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};

struct ObjTexCord
{
	double u = 0, v = 0, w = 1;

	inline double* _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};
struct ObjNormal
{
	double x = 0, y = 0, z = 0;
	inline double* _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};

struct ObjFace
{
	unsigned int VertexCount;

	std::list<ObjVertex> vertex;
	std::list<ObjTexCord> texCoord;
	std::list<ObjNormal> normal;
};

class ObjModel
{
	std::list<ObjFace> Faces;

	int listId = -1;

public:
	ObjModel() = default;   
	~ObjModel()
	{
		glDeleteLists(listId, 1);
	}

	inline void RenderModel(int mode);

	inline void Draw(GLenum mode = 0x0009) // GL_POLYGON
	{
		glCallList(listId);
	}

	int LoadModel(const char* filename);

	// ObjFile(const ObjFile)
};
