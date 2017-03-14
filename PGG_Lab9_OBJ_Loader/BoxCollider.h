#pragma once
#include "glm.hpp"					//the icnlude files
#include <memory>

class BoxCollider
{
public:
	BoxCollider();
	~BoxCollider();
	void SetUpCollider(std::shared_ptr<BoxCollider> _collider, float modelWidth);
	bool CheckForCollision(std::shared_ptr<BoxCollider> _collider, std::shared_ptr<BoxCollider> _collider2);			//the functions which you can access
	void UpdateCollider(glm::vec3 _position, std::shared_ptr<BoxCollider> _collider);

private:
	glm::vec3 corner1, corner2, corner3;
	glm::vec3 base1, base2, base3;					//the private variavles
};

