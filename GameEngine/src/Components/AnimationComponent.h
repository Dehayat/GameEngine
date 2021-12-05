#pragma once
#include <SDL.h>

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int frameRate;
	bool isLoop;
	double currentTime;

	AnimationComponent(int numFrames = 1, int frameRate = 1, bool isLoop = true) {
		this->numFrames = numFrames;
		this->currentFrame = 0;
		this->frameRate = frameRate;
		this->isLoop = isLoop;
		this->currentTime = 0;
	}
};