#include "GCube.h"

GCube::GCube(float x, float y, float z) : GObject()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData cube;
	geoGen.CreateBox3(x, y, z, cube);

	mVertexCount = cube.Vertices.size();
	mIndexCount = cube.Indices.size();

	mVertices.resize(mVertexCount);
	for (size_t i = 0; i < mVertexCount; ++i)
	{
		mVertices[i].Pos = cube.Vertices[i].Position;
		mVertices[i].Normal = cube.Vertices[i].Normal;
		mVertices[i].Tex = cube.Vertices[i].TexC;
		//		mVertices[i].TangentU = cube.Vertices[i].TangentU;
	}

	mIndices.resize(mIndexCount);
	mIndices.insert(mIndices.begin(), cube.Indices.begin(), cube.Indices.end());
}

GCube::~GCube()
{
}
