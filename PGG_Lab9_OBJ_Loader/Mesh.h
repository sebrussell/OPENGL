#ifndef __MESH__
#define __MESH__

#include <glm.hpp>
#include <SDL.h>					//the include files
#include "glew.h"
#include <string>

class Mesh
{
public:

	Mesh();
	~Mesh();
	
	void LoadOBJ( std::string filename );							//the public functions

	void Draw();

	float GetModelWidth() { return modelWidth; }			

	void SetAsCube();											//this is so you can set the mesh to be a cube (used for the skybox)


protected:
	
	GLuint _VAO;

	unsigned int _numVertices;			

	float modelWidth;						//this is used so the program knows how big the collision box should be

};


#endif
