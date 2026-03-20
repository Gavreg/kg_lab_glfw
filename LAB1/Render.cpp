#include "OpenGL.h"


void Render(double delta_time)
{

	glBegin(GL_TRIANGLES);

	glVertex3d(1, 1, 1);
	glVertex3d(5, 5, 1);
	glVertex3d(5, 1, 1);

	glEnd();

}