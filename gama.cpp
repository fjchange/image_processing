#include"BMP.h"
#include"gama.h"
#include<cmath>

BYTE* gray_inverse(BYTE*pImg, INT nWidth, INT nHeight) {
	BYTE* gray_inverse_yuv_pImg = RGB_YUV(pImg, nWidth, nHeight);
	BYTE *new_begin = gray_inverse_yuv_pImg;
	int i = 0;
	while (i<3 * nWidth*nHeight) {
		*new_begin = 255 - *new_begin;
		*(new_begin+1) = *(new_begin+1);
		*(new_begin+2) = *(new_begin+2);
		new_begin += 3, i += 3;
	}
	BYTE* gray_inverse_rgb_pImg = YUV_RGB(gray_inverse_yuv_pImg, nWidth,nHeight);
	return gray_inverse_rgb_pImg;
}

BYTE* inverse(BYTE*pImg, INT nWidth, INT nHeight) {
	//BYTE* gray_inverse_yuv_pImg = RGB_YUV(pImg, nWidth, nHeight);
	//BYTE *new_begin = gray_inverse_yuv_pImg;
	BYTE* gray_inverse_out_pImg = new BYTE[3 * nWidth*nHeight];
	BYTE*begin = pImg, *new_begin = gray_inverse_out_pImg;
	int i = 0;
	while (i<3 * nWidth*nHeight) {
		*new_begin = 255 - *begin;
		*(new_begin + 1) = 255 - *(begin + 1);
		*(new_begin + 2) = 255 - *(begin + 2);
		new_begin += 3, i += 3;
		begin += 3;
	}
	return gray_inverse_out_pImg;
}


BYTE* gama_trans(BYTE*pImg, float gama, int nWidth, int nHeight) {
	BYTE pixel_map[256];
	BYTE* gama_out_pImg = new BYTE[3 * nWidth*nHeight];

	BYTE* begin = pImg, *new_begin = gama_out_pImg;
	for (int i = 0; i < 256; i++) {
		int pixel_val = int(pow(float(i/255.0), 1.0/gama)*255.0);
		pixel_map[i] = avoid_overscale(pixel_val);
	}
	int i = 0;
	while (i < 3 * nWidth*nHeight) {
		*new_begin = pixel_map[*begin];
		*(new_begin+1) = pixel_map[*(begin+1)];
		*(new_begin+2) = pixel_map[*(begin+2)];
		new_begin += 3;
		begin += 3;
		i += 3;
	}
	return gama_out_pImg;
}