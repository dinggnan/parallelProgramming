#include "edge_detect.h"
#include <algorithm>
void erode(uint8_t* min_img, const uint8_t* gray_img, const int width, const int height, const int edge_width)
{

	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int left = std::max(0, x - edge_width);
			const int right = std::min(width-1, x + edge_width);
			const int up = std::max(0, y - edge_width);
			const int down = std::min(height - 1, y + edge_width);

			// Find min and max!
			uint8_t min = 255;
			for (int i = up; i <= down; i++) {
				for (int j = left; j <= right; j++) {
					const uint8_t pixel = gray_img[j+i*width];
					if (pixel < min) {
						min = pixel;
					}
				}
			}
            min_img[x+y*width] = min;
		}
	}

}
void dilate(uint8_t* max_img, const uint8_t* gray_img, const int width, const int height, const int edge_width) {
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int left = std::max(0, x - edge_width);
			const int right = std::min(width-1, x + edge_width);
			const int up = std::max(0, y - edge_width);
			const int down = std::min(height - 1, y + edge_width);

			// Find min and max!
			uint8_t max = 0;
			for (int i = up; i <= down; i++) {
				for (int j = left; j <= right; j++) {
					const uint8_t pixel = gray_img[j+i*width];
					if (pixel > max) {
						max = pixel;
					}
				}
			}

			// Compute final value
			max_img[x+y*width] = max;
		}
	}
}

void dilateanderode(uint8_t* min_img, uint8_t* max_img, const uint8_t* gray_img, const int width, const int height, const int edge_width) 
{
	//#pragma omp parallel for
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const int left = std::max(0, x - edge_width);
			const int right = std::min(width-1, x + edge_width);
			const int up = std::max(0, y - edge_width);
			const int down = std::min(height - 1, y + edge_width);

			// Find min and max!
			uint8_t max = 0;
			uint8_t min = 255;
			for (int i = up; i <= down; i++) {
				for (int j = left; j <= right; j++) {
					const uint8_t pixel = gray_img[j+i*width];
					if (pixel < min) {
						min = pixel;
					}
					if (pixel > max) {
						max = pixel;
					}
				}
			}
            min_img[x+y*width] = min;
            max_img[x+y*width] = max;
		}
	}

}

void edge_detect(uint8_t* gray_dst, const uint8_t* gray_img, const int width, const int height, const int edge_width)
{
    uint8_t* min_img = new uint8_t [width*height];
    //erode(min_img, gray_img, width, height, edge_width);
    uint8_t* max_img = new uint8_t [width*height];
    //#pragma omp task
    dilateanderode(min_img,max_img, gray_img, width, height, edge_width);
   // dilate(max_img, gray_img, width, height, edge_width);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// Compute final value
						
			gray_dst[x+y*width] = max_img[x+y*width] - min_img[x+y*width];
		}
	}
    delete [] min_img;
    delete [] max_img;
}
