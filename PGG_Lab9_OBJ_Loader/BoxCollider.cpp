#include "BoxCollider.h"



BoxCollider::BoxCollider()
{

}


BoxCollider::~BoxCollider()		
{
}

void BoxCollider::SetUpCollider(std::shared_ptr<BoxCollider> _collider, float modelWidth)
{
	_collider->corner1 = glm::vec3(0, 0, 0) * modelWidth;
	_collider->corner2 = glm::vec3(1, 1, 0) * modelWidth;				//the size of the collider box is a unit cube * model with 
	_collider->corner3 = glm::vec3(0, 0, 1) * modelWidth;

	_collider->base1 = _collider->corner1;
	_collider->base2 = _collider->corner2;								//keep track of the original size
	_collider->base3 = _collider->corner3;
}

bool BoxCollider::CheckForCollision(std::shared_ptr<BoxCollider> _collider, std::shared_ptr<BoxCollider> _collider2)
{
	if (_collider2->corner1.x >= _collider->corner1.x && _collider2->corner2.x <= _collider->corner1.x)
	{
		if (_collider2->corner1.y >= _collider->corner1.y && _collider2->corner2.y <= _collider->corner2.x)
		{
			if (_collider2->corner3.z >= _collider->corner3.z && _collider2->corner1.z <= _collider->corner1.z)
			{
				return true;	///dont think this works, need to check the if statements later on 
			}
		}
	}
	return false;
}

void BoxCollider::UpdateCollider(glm::vec3 _position, std::shared_ptr<BoxCollider> _collider)
{
	_collider->corner1 = _collider->base1;
	_collider->corner2 = _collider->base2;					//temporarily reset the box collider to the origin
	_collider->corner3 = _collider->base3;

	_collider->corner1 += _position;						//works out the new position of the box collider
	_collider->corner2 += _position;
	_collider->corner3 += _position;

}
