#pragma once
#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();
	}
	void Update(double deltaTime) {
		for (auto entity : GetSystemEntities()) {
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& animationComponent = entity.GetComponent<AnimationComponent>();
			animationComponent.currentTime += deltaTime;
			animationComponent.currentFrame =
				animationComponent.currentTime * animationComponent.frameRate;

			if (animationComponent.currentFrame >= animationComponent.numFrames) {
				double fullAnimationTime = (double)animationComponent.numFrames / animationComponent.frameRate;
				animationComponent.currentTime -= fullAnimationTime;
				animationComponent.currentFrame =
					animationComponent.currentTime * animationComponent.frameRate;
			}
			sprite.srcRect.x = sprite.srcRect.w * animationComponent.currentFrame;
		}
	}
};
