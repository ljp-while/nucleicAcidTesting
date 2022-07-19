#include "imageProcess.h" 
#include "flash.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

uint16_t threshold = 0;
uint16_t topLine = 0;
uint16_t bottomLine = 0;
uint16_t charXlocation[2][2] = {{0,0}};
RunMode currentMode;

static void draw_vertical_line(uint16_t top, uint16_t left, uint16_t right, uint8_t *buff)
{
	uint8_t lineCount = 0;
	uint16_t count = 0;
	uint16_t width = 240 * 2;
//	uint16_t height = 320 * 2;
	uint8_t sectorsStart = width * top / W25Q128_SECTION_LENGTH;
	uint16_t startWz = width * top % W25Q128_SECTION_LENGTH;
	sectorsStart += 40;
	EN25QXX_Read(buff, sectorsStart * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = (startWz + left * 2) ; count < W25Q128_SECTION_LENGTH; count+=width)
	{
		buff[count] = 0xF8;
		buff[count + 1] = 0x00;
		lineCount++;
	}
	w25q128_erase_sector(sectorsStart*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsStart*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
	
	sectorsStart += 1;
	startWz = ( width * (top + lineCount) ) % W25Q128_SECTION_LENGTH;
	EN25QXX_Read(buff, sectorsStart * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = (startWz + left * 2); count < W25Q128_SECTION_LENGTH; count+=width)
	{
		buff[count] = 0xF8;
		buff[count + 1] = 0x00;
	}
	w25q128_erase_sector(sectorsStart*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsStart*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
	
	lineCount = 0;
	sectorsStart = width * top / W25Q128_SECTION_LENGTH;
	sectorsStart += 40;
	startWz = width * top % W25Q128_SECTION_LENGTH;
	EN25QXX_Read(buff, sectorsStart * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = (startWz + right * 2) ; count < W25Q128_SECTION_LENGTH; count+=width)
	{
		buff[count] = 0xF8;
		buff[count + 1] = 0x00;
		lineCount++;
	}
	w25q128_erase_sector(sectorsStart*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsStart*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
	
	sectorsStart += 1;
	startWz = ( width * (top + lineCount) ) % W25Q128_SECTION_LENGTH;
	EN25QXX_Read(buff, sectorsStart * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = (startWz + right * 2); count < W25Q128_SECTION_LENGTH; count+=width)
	{
		buff[count] = 0xF8;
		buff[count + 1] = 0x00;
	}
	w25q128_erase_sector(sectorsStart*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsStart*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
}

static void  draw_transverse_line(uint16_t top, uint16_t bottom, uint8_t *buff)
{
	uint16_t count = 0;
	uint16_t width = 240 * 2;
	uint8_t sectorsStart = width * top / W25Q128_SECTION_LENGTH;
	uint8_t sectorsEnd = width * bottom / W25Q128_SECTION_LENGTH;
	uint16_t startWz = width * top % W25Q128_SECTION_LENGTH;
	sectorsStart += 40;
	sectorsEnd += 40;
	EN25QXX_Read(buff, sectorsStart * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = 0; count < 240; count++)
	{
		buff[startWz++] = 0xF8;
		buff[startWz++] = 0x00;
	}
	w25q128_erase_sector(sectorsStart*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsStart*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
	
	EN25QXX_Read(buff, sectorsEnd * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
	for(count = 0; count < 240; count++)
	{
		buff[startWz++] = 0xF8;
		buff[startWz++] = 0x00;
	}
	w25q128_erase_sector(sectorsEnd*W25Q128_SECTION_LENGTH,1);  //����һ������
	w25q128_write_no_check(sectorsEnd*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
}

void image_rollovers(int width, int heigth, uint8_t *buff)
{
	int line = 0;
	int rows = 0;	
	uint8_t color[2] = { 0 };
	uint16_t count = 0;
	uint8_t sectorsCount = 40;
	for(rows = width - 1; rows >= 0; rows--)
	{
		for(line = 0; line < heigth; line++)
		{
			EN25QXX_Read(color, line*width*2+rows*2, 2);  //��ȡһ������
			buff[count++] =  color[0];
			buff[count++] =  color[1];
			if(count == W25Q128_SECTION_LENGTH)
			{
				w25q128_erase_sector(sectorsCount*W25Q128_SECTION_LENGTH,1);  //����һ������
				w25q128_write_no_check(sectorsCount*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);  //д��һ������
				count = 0;
				sectorsCount++;
			}
		}
	}
	if(count != 0)
	{
		w25q128_erase_sector(sectorsCount*W25Q128_SECTION_LENGTH,1);  //����һ������
		w25q128_write_no_check(sectorsCount*W25Q128_SECTION_LENGTH, buff, count);  //д��һ������		
	}
}

void rgb565_to_gray(int width, int heigth, uint8_t *buff)
{
//	uint16_t line = 0;
	uint16_t rows = 0;
	uint16_t color=0;
	uint8_t gray = 0;
	uint8_t RGB_R,RGB_G,RGB_B;
	uint32_t imageLength = width * heigth * 2;
	uint16_t sectorCount = imageLength / W25Q128_SECTION_LENGTH;
	uint8_t count = 0;
	if(imageLength % W25Q128_SECTION_LENGTH != 0)
	{
		sectorCount += 1;
	}

	for(count = 0; count < sectorCount; count++)
	{
		EN25QXX_Read(buff, OVERTURN_IMAGE_ADDRESS + count * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
		for(rows = 0; rows < 2048; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			RGB_R=(uint8_t)((color & 0xF800) >> 8);						//��ȡR G B
			RGB_G=(uint8_t)((color & 0x07E0) >> 3);
			RGB_B=(uint8_t)((color & 0x001F) << 3);
			gray=(uint8_t)((RGB_R*30 + RGB_G*59 + RGB_B*11 + 50) / 100) ;	 
			color = (((gray&0xF8)<<8)|((gray&0xFC) <<3)|(gray&0xF8)>>3);	
			buff[rows * 2] = color & 0xff;   //color & 0xff
			buff[rows * 2 + 1] = (color >> 8) & 0xff;  //(color >> 8) & 0xff			
		}
		w25q128_erase_sector(OVERTURN_IMAGE_ADDRESS+count*W25Q128_SECTION_LENGTH,1);  //����flashоƬ
		w25q128_write_no_check(OVERTURN_IMAGE_ADDRESS+count*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);
	}
	


}

void gray_to_bin(int width, int heigth, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint32_t imageStart = width * y0 * 2;
	uint32_t imageLength = width * (y0 + h) * 2;
	uint16_t sectorCount = imageLength / W25Q128_SECTION_LENGTH;
	imageStart /= W25Q128_SECTION_LENGTH;
	int rows = 0;	
	uint16_t color=0;
	int thresholdValue = threshold * 256;
	uint8_t count = 0;
	if(imageLength % W25Q128_SECTION_LENGTH != 0)
	{
		sectorCount += 1;
	}

	for(count = imageStart; count < sectorCount; count++)
	{
		EN25QXX_Read(buff, OVERTURN_IMAGE_ADDRESS+count * W25Q128_SECTION_LENGTH, W25Q128_SECTION_LENGTH);  //��ȡһ������
		for(rows = 0; rows < 2048; rows++)
		{	
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			color = color > thresholdValue ? 0xffff : 0;
			buff[rows * 2] = color & 0xff;
			buff[rows * 2 + 1] = (color >> 8) & 0xff;				
		}
		w25q128_erase_sector(OVERTURN_IMAGE_ADDRESS+count*W25Q128_SECTION_LENGTH,1);  //����flashоƬ
		w25q128_write_no_check(OVERTURN_IMAGE_ADDRESS+count*W25Q128_SECTION_LENGTH, buff, W25Q128_SECTION_LENGTH);
	}
}

uint8_t char_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff)
{

	top_bottom_location(width, height, x0, y0, w, h, buff);
	char_segmentation(width, height, x0, y0, w, h, buff);

	//	draw_transverse_line(y0,y0+h,buff);
//	draw_vertical_line(y0,x0,x0+w,buff);
//	
//	draw_transverse_line(topLine,bottomLine,buff);
//	draw_transverse_line(topLine + 57,0,buff);   //���� 
//	draw_transverse_line(topLine + 80,0,buff);   //�ײ�
//	draw_vertical_line(y0,charXlocation[0][0],charXlocation[0][1],buff);
//	draw_vertical_line(y0,charXlocation[1][0],charXlocation[1][1],buff);
//	draw_vertical_line(topLine + 57,charXlocation[0][0] + C_DETECTION_AREA_LEFT,charXlocation[0][0] + C_DETECTION_AREA_RIGHT,buff);
//	draw_vertical_line(topLine + 57,charXlocation[1][0] + C_DETECTION_AREA_LEFT,charXlocation[1][0] + C_DETECTION_AREA_RIGHT,buff);
//	_DEBUG_PRINT_("y0 =%d y0 + h=%d\r\n",y0, y0 + h);
//	_DEBUG_PRINT_("topLine =%d bottomLine=%d\r\n",topLine, bottomLine);
//	_DEBUG_PRINT_("charXlocation = %d %d %d %d\r\n",charXlocation[0][0], charXlocation[0][1], charXlocation[1][0], charXlocation[1][1]);	

	if(topLine < y0 || topLine > y0 + h)
	{
		return 0;
	}

	if(bottomLine < topLine || bottomLine > y0 + h)
	{
		return 0;
	}
	
	if(bottomLine - topLine < 10)
	{
		return 0;
	}
	if(charXlocation[0][1] - charXlocation[0][0] < 10)
	{
		return 0;
	}
	if(charXlocation[1][1] - charXlocation[1][0] < 10)
	{
		return 0;
	}
	return 1;
}


uint8_t t_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint16_t color=0;
	uint8_t rt = 0;
	uint32_t gray1 = 0;
	uint32_t gray2 = 0;
	uint32_t gray = 0;
	uint32_t grayValue = 0;
	int line = 0;
	int rows = 0;	
	int value = (T_DETECTION_AREA_RIGHT - T_DETECTION_AREA_LEFT) * (DETECTION_AREA_BOTTOM - DETECTION_AREA_TOP);
	uint8_t length = T_DETECTION_AREA_RIGHT - T_DETECTION_AREA_LEFT;
	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{
		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[1][0] - length - 3; rows < charXlocation[1][0] - 3; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			gray1 += color;    					//��ȡ���ص�
		}
	}


	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{
		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[1][1] + 3; rows < charXlocation[1][1] + length + 3; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			gray2 += color;    					//��ȡ���ص�
		}
	}	
	
	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{
		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[1][0] + T_DETECTION_AREA_LEFT; rows < charXlocation[1][0] + T_DETECTION_AREA_RIGHT; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			grayValue += color;    					//��ȡ���ص�
		}
	}
	grayValue /= value;
	gray1 /= value;
	gray2 /= value;
	gray = (gray1 + gray2) / 2;
	if(gray > grayValue)
	{
		if(gray - grayValue > LIMIT_THRESHOLD)
		{
			rt = 1;
		}
		else
		{
			rt = 0;
		}		
	}
	else
	{
		rt = 0;
	}

	_DEBUG_PRINT_("T  grayValue = %d gray = %d %d\r\n",grayValue, gray, gray - grayValue);
	return rt;
}


uint8_t c_gray_value_statistics(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint16_t color=0;
	uint8_t rt = 0;
	uint32_t gray1 = 0;
	uint32_t gray2 = 0;
	uint32_t gray = 0;
	uint32_t grayValue = 0;
	int line = 0;
	int rows = 0;	
	int value = (C_DETECTION_AREA_RIGHT - C_DETECTION_AREA_LEFT) * (DETECTION_AREA_BOTTOM - DETECTION_AREA_TOP);
	uint8_t length = C_DETECTION_AREA_RIGHT - C_DETECTION_AREA_LEFT;
	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{

		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[0][0] - length - 3; rows < charXlocation[0][0] - 3; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			gray1 += color;    					//��ȡ���ص�  gray1 += LCD_ReadPoint(rows, line); 
		}
	}

	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{

		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[0][1] + 3; rows < charXlocation[0][1] + length + 3; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			gray2 += color;    					//��ȡ���ص�  gray1 += LCD_ReadPoint(rows, line); 
		}
	}
	
	for(line = topLine + DETECTION_AREA_TOP; line < topLine + DETECTION_AREA_BOTTOM; line++)
	{
		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = charXlocation[0][0] + C_DETECTION_AREA_LEFT; rows < charXlocation[0][0] + C_DETECTION_AREA_RIGHT; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			grayValue += color;    					//��ȡ���ص�
		}
	}
	grayValue /= value;
	gray1 /= value;
	gray2 /= value;
	gray = (gray1 + gray2) / 2;
	if(gray > grayValue)
	{
		if(gray - grayValue > LIMIT_THRESHOLD)
		{
			rt = 1;
		}
		else
		{
			rt = 0;
		}		
	}
	else
	{
		rt = 0;
	}

	_DEBUG_PRINT_("C  grayValue = %d gray = %d %d\r\n",grayValue, gray, gray - grayValue);
	return rt;
}

static void char_segmentation(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint8_t saltusBuff[320] = { 0 };
	int line = 0;
	int rows = 0;	
	uint16_t color = 0;
	uint16_t colorNext = 0xffff;
	memset(saltusBuff, 0, sizeof(saltusBuff));	
	for(rows = x0; rows < x0 + w; rows++)   //for(rows = x0; rows < x0 + w; rows++)
	{
			
		for(line = y0; line < y0 + h; line++)
		{	
			EN25QXX_Read((uint8_t*)buff,40 * W25Q128_SECTION_LENGTH + line * width * 2 + rows * 2,2);  //��ȡһ������		
			color = (buff[1] << 8) | buff[0];
			if(color != colorNext)
			{
				saltusBuff[rows]++;
			}
			colorNext = color;
		}				
	}
		char_X_coordinate_location(saltusBuff, sizeof(saltusBuff));
}
	
static void char_X_coordinate_location(uint8_t *buff, uint16_t size)
{

	uint16_t count = 0;
	while(count < size)
	{
		if(buff[count] != 0)
		{
			charXlocation[0][0] = count;
			charXlocation[0][0] -= 1;
			break;
		}
		count++;
	}
	count++;
	while(count < size)
	{
		if(buff[count] == 0)
		{
			charXlocation[0][1] = count;
			charXlocation[0][1] += 1;
			break;
		}
		count++;
	}
	count++;
	
	while(count < size)
	{
		if(buff[count] != 0)
		{
			charXlocation[1][0] = count;
			charXlocation[1][0] -= 1;
			break;
		}
		count++;
	}
	count++;
	while(count < size)
	{
		if(buff[count] == 0)
		{
			charXlocation[1][1] = count;
			charXlocation[1][1] += 1;
			break;
		}
		count++;
	}
	count++;
}

static void top_bottom_location(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint8_t saltusBuff[240] = { 0 };
	int line = 0;
	int rows = 0;	
	uint16_t color = 0;
	uint16_t colorNext = 0xffff;
	memset(saltusBuff, 0, sizeof(saltusBuff));
	for(line = y0; line < y0 + h; line++)
	{
		EN25QXX_Read((uint8_t*)buff,40 * W25Q128_SECTION_LENGTH + line*width*2,width*2);  //��ȡһ��saltusCount = 0;
		for(rows = x0; rows < x0 + w; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			if(color != colorNext)
			{
				saltusBuff[line]++;
			}
			colorNext = color;			
		}
		
	}	
	find_top_line(saltusBuff, sizeof(saltusBuff));
	fine_bottom_line(saltusBuff, sizeof(saltusBuff));
}

static void find_top_line(uint8_t *buff, uint8_t size)
{
	uint8_t count = 0;
	for(count = 0; count < size; count++)
	{
		if(buff[count] > 2)
		{
			topLine = count;
			//topLine -= 1;
			return;
		}
	}
}
	
static void fine_bottom_line(uint8_t *buff, uint8_t size)
{
	uint8_t count = 0;
	for(count = size - 1; count > 0; count--)
	{
		if(buff[count] > 2)
		{
			bottomLine = count;
			//bottomLine += 1;
			return;
		}
	}	
}

void sobel_process(uint16_t width, uint16_t height, uint8_t *pBmpBuf)
{
//	threshold += 0;
//	int thresholdValue = threshold * 256;
//    int ul, uc, ur, dl, dc, dr;
//	int lu, lc, ld, ru, rc, rd;
//	double hir, vec;
//	uint16_t x = 0;
//	uint16_t y = 0;
//	uint16_t colorBuff[2048] = { 0 };
//	uint16_t colorBuffData[2048] = { 0 };
//	uint16_t line = 0;
//	uint16_t rows = 0;
//	uint32_t value = 0;
//	uint32_t lineByte = width;
//	for (y = 1; y < height - 1; y++)
//	{
//		EN25QXX_Read((uint8_t*)pBmpBuf,(y-1)*width*2,width*2*3);  //��ȡ����
//		for( line = 0; line < 3; line++)
//		{
//			for( rows = 0; rows < width; rows++)
//			{
//				colorBuff[line * width + rows] = (pBmpBuf[ line * width + rows * 2 + 1] << 8) | pBmpBuf[ line * width + rows * 2 ];
//			}
//		}


//		for (x = 1; x < width - 1; x++)
//		{
//			// ��ֱ�ݶ����ӣ����ˮƽ��Ե
//			vec = 0;
//			ul = *(colorBuff + lineByte * (1 + 1) + (x - 1))*(-1);
//			uc = *(colorBuff + lineByte * (1 + 1) + x)*(-2);
//			ur = *(colorBuff + lineByte * (1 + 1) + (x+1))*(-1); //(x + 1)
//			dl = *(colorBuff + lineByte * (1 - 1) + (x - 1)) * 1;
//			dc = *(colorBuff + lineByte * (1 - 1) + x) * 2;
//			dr = *(colorBuff + lineByte * (1 - 1) + (x+1)) * 1; //(x + 1)
//			vec = ul + uc + ur + dl + dc + dr;
//			// ˮƽ�ݶ����ӣ���ⴹֱ��Ե
//			hir = 0;
//			lu = *(colorBuff + lineByte * (1 + 1) + (x - 1))*(-1);
//			lc = *(colorBuff + lineByte * (1 - 0) + (x - 1))*(-2);
//			ld = *(colorBuff + lineByte * (1 - 1) + (x - 1))*(-1);
//			ru = *(colorBuff + lineByte * (1 + 1) + (x + 1)) * 1;
//			rc = *(colorBuff + lineByte * (1 - 0) + (x + 1)) * 2;
//			rd = *(colorBuff + lineByte * (1 - 1) + (x + 1)) * 1;
//			hir = lu + lc + ld + ru + rc + rd;
//			value = abs(vec) + abs(hir); //+ abs(hir)
//			if(value > thresholdValue)
//			{
//				value = 0xffff;
//			}
//			else
//			{
//				value = 0;
//			}
//			colorBuffData[x] = value;
//		}

////		LCD_ShowPicture(0, y , width, 1, (uint8_t *)colorBuff);   //��ʾһ��
////		LCD_ShowPicture(0, y , width, 1, (uint8_t *)colorBuffData);   //��ʾһ��
//	}

}

void histogram_distribution(int width, int heigth, int x0, int y0, int w, int h, uint8_t *buff)
{
	uint16_t pixel[256]={0};
	int line = 0;
	int rows = 0;
	uint16_t color=0;
	for(line = y0; line < y0 + h; line++)
	{
		EN25QXX_Read((uint8_t*)buff,OVERTURN_IMAGE_ADDRESS+line*width*2,width*2);  //��ȡһ��
		for(rows = x0; rows < x0 + w; rows++)
		{
			color = (buff[rows * 2 + 1] << 8) | buff[rows * 2];
			pixel[color/256]++;						//��16bitλ���ص�  ��СΪ8bit
		}
	}
	otsu_function(w, h, pixel);
}


/*
*	Ѱ����ֵ
*/
void otsu_function(int width, int heigth, uint16_t* pixel)
{
	double w0 = 0.0;
    double w1 = 0.0;
    double u0_temp = 0.0;
    double u1_temp = 0.0;
    double u0 = 0.0;
    double u1 = 0.0;
    double delta_temp = 0.0;
    double delta_max = 0.0;
	float pixel_pro[256] = { 0.0 };
	uint16_t count=0;
	uint16_t i,j;
	uint32_t value= width * heigth;
	for(count=0;count<256;count++)
	{
		pixel_pro[count]=pixel[count]*1.0/value;
		
	}
	for(i=0;i<256;i++)
	{
		w0 = w1 = u0_temp = u1_temp = u0 = u1 = delta_temp = 0.0;
		for(j=0;j<256;j++)
		{
            if (j <= i)   //��������  
            {
                w0 += pixel_pro[j];//�������ر���
                u0_temp += j * pixel_pro[j];
            }
            else   //ǰ������  
            {
                w1 += pixel_pro[j];//ǰ�����ر���
                u1_temp += j * pixel_pro[j];
            }			
		}
		u0 = u0_temp / w0;//�������ص��ƽ���Ҷ�
        u1 = u1_temp / w1;//ǰ�����ص��ƽ���Ҷ�
        delta_temp = (double)(w0 *w1*(u0 - u1)*(u0 - u1));//��䷽�� g=w0*w1*(u0-u1)^2
        //����䷽��delta_temp���ʱ����Ӧ��i������ֵT
        if (delta_temp > delta_max)
        {
            delta_max = delta_temp;
            threshold = i;
        }
	}
}

void image_data_analysis(uint16_t width, uint16_t height, int x0, int y0, int w, int h, uint8_t *jpeg_buf)
{
	uint8_t state = 0;
	uint8_t cCharState = 0;
	uint8_t tCharState = 0;
	image_rollovers(IMAGE_WIDTH, IMAGE_HIGHT, (uint8_t*)jpeg_buf);  //����תͼ��
	rgb565_to_gray(IMAGE_HIGHT, IMAGE_WIDTH, (uint8_t*)jpeg_buf);  //rgbת�Ҷ�
	histogram_distribution(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //Ѱ�Ҷ�ֵ����ֵ
	gray_to_bin(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);   //C T��ֵ��
	state = char_location(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //��λC Tλ��
	if(state == 1)
	{
		cCharState = c_gray_value_statistics(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //C�ַ�����  �Ҷ�ֵ����
		tCharState = t_gray_value_statistics(IMAGE_HIGHT, IMAGE_WIDTH, X_START, Y_START, X_WIDTH, Y_HIGHT, (uint8_t*)jpeg_buf);  //T�ַ�����  �Ҷ�ֵ����
		if(cCharState == 0)    //��Ч
		{
			lcd_display_16x16_char(4, 60, 'i');
		}
		else if(cCharState == 1 && tCharState == 0)  //����
		{
			lcd_display_16x16_char(4, 60, '-');
		}
		else if(cCharState == 1 && tCharState == 1)   //����
		{
			lcd_display_16x16_char(4, 60, '+');
		}
	}
	else   //��λʧ��
	{
		lcd_display_16x16_char(4, 60, 'e');
	}	
}
