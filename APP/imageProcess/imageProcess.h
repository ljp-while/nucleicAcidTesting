#ifndef  __IMAGEPROCESS_H
#define  __IMAGEPROCESS_H

#include "main.h"


#define DETECTION_AREA_TOP      53   
#define DETECTION_AREA_BOTTOM   78
#define C_DETECTION_AREA_LEFT   7
#define C_DETECTION_AREA_RIGHT  16

#define T_DETECTION_AREA_LEFT   7
#define T_DETECTION_AREA_RIGHT  16

#define LIMIT_THRESHOLD         650  //灰度分析阈值


typedef struct{
	uint8_t autoMode;
	uint8_t manualMode;
}RunMode;

extern RunMode currentMode;





static void top_bottom_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
static void char_segmentation(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
static void char_X_coordinate_location(uint8_t *buff, uint16_t size);
static void fine_bottom_line(uint8_t *buff, uint8_t size);
static void find_top_line(uint8_t *buff, uint8_t size);
static void draw_transverse_line(uint16_t top, uint16_t bottom, uint8_t *buff);
static void draw_vertical_line(uint16_t top, uint16_t left, uint16_t right, uint8_t *buff);

/*! \brief  RGB图像转灰度
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param buff   缓存区
 */
void rgb565_to_gray(int width, int heigth, uint8_t *buff);

/*! \brief  图像选择
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param buff   缓存区
 */
void image_rollovers(int width, int heigth, uint8_t *buff);


/*! \brief  直方图统计
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 */
void histogram_distribution(int width, int heigth, int x0, int y0, int w, int h,uint8_t *buff);


/*! \brief  使用otsu寻找合适的二值化阈值
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 */
void otsu_function(int width, int heigth, uint16_t* pixel);


/*! \brief  图像二值化
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 */
void gray_to_bin(int width, int heigth, int x0, int y0, int w, int h, uint8_t *buff);


void sobel_process(uint16_t width, uint16_t heigth, uint8_t *imageBuff);


/*! \brief  定位C和T字符
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 * \return 返回‘1’表示定位成功，‘0’表示定位失败
 */
uint8_t char_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  C字符的横线灰度分析
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 * \return 返回‘1’表示有横线，‘0’表示无横线
 */
uint8_t c_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  T字符的横线灰度分析
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 * \return 返回‘1’表示有横线，‘0’表示无横线
 */
uint8_t t_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  分析图像数据
 *
 * \param width  图像宽度
 * \param height 图像高度
 * \param x0     待分析区域的x0坐标
 * \param y0     待分析区域的y0坐标
 * \param w      待分析区域的宽度
 * \param h      待分析区域的高度
 * \param buff   缓存区
 */
void image_data_analysis(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
#endif /*__IMAGEPROCESS_H*/

