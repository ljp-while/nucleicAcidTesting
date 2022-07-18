#ifndef  __IMAGEPROCESS_H
#define  __IMAGEPROCESS_H

#include "main.h"

typedef enum{
	DETECT_INVALID = 0,
	DETECT_NEGATIVE,
	DETECT_POSITIVE,
}DetectState;


#define DETECTION_AREA_TOP      53
#define DETECTION_AREA_BOTTOM   78
#define C_DETECTION_AREA_LEFT   7
#define C_DETECTION_AREA_RIGHT  16

#define T_DETECTION_AREA_LEFT   7
#define T_DETECTION_AREA_RIGHT  16

#define LIMIT_THRESHOLD         650


static void top_bottom_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
static void char_segmentation(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
static void char_X_coordinate_location(uint8_t *buff, uint16_t size);
static void fine_bottom_line(uint8_t *buff, uint8_t size);
static void find_top_line(uint8_t *buff, uint8_t size);
static void draw_transverse_line(uint16_t top, uint16_t bottom, uint8_t *buff);
static void draw_vertical_line(uint16_t top, uint16_t left, uint16_t right, uint8_t *buff);
void rgb565_to_gray(int width, int heigth, uint8_t *buff);
void image_rollovers(int width, int heigth, uint8_t *buff);
void histogram_distribution(int width, int heigth, int x0, int y0, int w, int h,uint8_t *buff);
void otsu_function(int width, int heigth, uint16_t* pixel);
void gray_to_bin(int width, int heigth, int x0, int y0, int w, int h, uint8_t *buff);
void sobel_process(uint16_t width, uint16_t heigth, uint8_t *imageBuff);
uint8_t char_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
uint8_t c_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
uint8_t t_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
#endif /*__IMAGEPROCESS_H*/

