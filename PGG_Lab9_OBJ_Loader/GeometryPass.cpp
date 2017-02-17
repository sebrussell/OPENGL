#include "GeometryPass.h"

GeometryPass::GeometryPass()
{
}

GeometryPass::~GeometryPass()
{

}

bool GeometryPass::Init()
{
	//if (!AddShader(GL_VERTEX_SHADER, "shaders/geometry_pass.vs")) {
	//	return false;
	//}


	//if (!AddShader(GL_FRAGMENT_SHADER, "shaders/geometry_pass.fs")) {
	//	return false;
	//}

	//m_WVPLocation = GetUniformLocation("gWVP");
	//m_WorldMatrixLocation = GetUniformLocation("gWorld");
	//m_colorTextureUnitLocation = GetUniformLocation("gColorMap");

	//if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
	//	m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
	//	m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION) {
	//	return false;
	//}

	return true;
}


void GeometryPass::SetWVP(const glm::fmat4& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, glm::value_ptr(WVP));
}


void GeometryPass::SetWorldMatrix(const glm::fmat4& worldInverse)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, glm::value_ptr(worldInverse));
}


void GeometryPass::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureUnitLocation, TextureUnit);
}

