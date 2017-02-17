#pragma once
#include "DeferredRendering.h"

class GeometryPass
{
public:
	GeometryPass();
	~GeometryPass();
	bool Init();
	void SetWVP(const glm::mat4& WVP);
	void SetWorldMatrix(const glm::mat4& WVP);
	void SetColorTextureUnit(unsigned int TextureUnit);

private:
	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_colorTextureUnitLocation;
};

