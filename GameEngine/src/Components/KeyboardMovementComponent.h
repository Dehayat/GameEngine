#pragma once
#include <GLM/glm.hpp>

struct KeyboardMovementComponent {
public:
	float moveSpeed;

	KeyboardMovementComponent(float moveSpeed = 10.0f) {
		this->moveSpeed = moveSpeed;
	}
};