#include <iostream>
#include <fstream>
#include <cstdlib>

void alpha_set(unsigned char* image, int width, int height, unsigned char alpha) {
	const size_t N = 4 * width * height;

	for (size_t i = 3; i < N; i += 4) {
		image[i] = alpha;
	}
}
