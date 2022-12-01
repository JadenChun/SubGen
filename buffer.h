#pragma once
#include <vector>

struct buffer {
	std::vector<float> dataBuffer;

	void Buffer(std::vector<float> source) {
		dataBuffer = source;
	}

	void Buffer() {}
};