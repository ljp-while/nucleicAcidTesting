#ifndef  __IMAGEPROCESS_H
#define  __IMAGEPROCESS_H

#include "main.h"


#define DETECTION_AREA_TOP      53   
#define DETECTION_AREA_BOTTOM   78
#define C_DETECTION_AREA_LEFT   7
#define C_DETECTION_AREA_RIGHT  16

#define T_DETECTION_AREA_LEFT   7
#define T_DETECTION_AREA_RIGHT  16

#define LIMIT_THRESHOLD         650  //�Ҷȷ�����ֵ


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

/*! \brief  RGBͼ��ת�Ҷ�
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param buff   ������
 */
void rgb565_to_gray(int width, int heigth, uint8_t *buff);

/*! \brief  ͼ��ѡ��
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param buff   ������
 */
void image_rollovers(int width, int heigth, uint8_t *buff);


/*! \brief  ֱ��ͼͳ��
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 */
void histogram_distribution(int width, int heigth, int x0, int y0, int w, int h,uint8_t *buff);


/*! \brief  ʹ��otsuѰ�Һ��ʵĶ�ֵ����ֵ
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 */
void otsu_function(int width, int heigth, uint16_t* pixel);


/*! \brief  ͼ���ֵ��
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 */
void gray_to_bin(int width, int heigth, int x0, int y0, int w, int h, uint8_t *buff);


void sobel_process(uint16_t width, uint16_t heigth, uint8_t *imageBuff);


/*! \brief  ��λC��T�ַ�
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 * \return ���ء�1����ʾ��λ�ɹ�����0����ʾ��λʧ��
 */
uint8_t char_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  C�ַ��ĺ��߻Ҷȷ���
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 * \return ���ء�1����ʾ�к��ߣ���0����ʾ�޺���
 */
uint8_t c_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  T�ַ��ĺ��߻Ҷȷ���
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 * \return ���ء�1����ʾ�к��ߣ���0����ʾ�޺���
 */
uint8_t t_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);


/*! \brief  ����ͼ������
 *
 * \param width  ͼ����
 * \param height ͼ��߶�
 * \param x0     �����������x0����
 * \param y0     �����������y0����
 * \param w      ����������Ŀ��
 * \param h      ����������ĸ߶�
 * \param buff   ������
 */
void image_data_analysis(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff);
#endif /*__IMAGEPROCESS_H*/

