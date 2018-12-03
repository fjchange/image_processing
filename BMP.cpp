#include"BMP.h"

BYTE* FAULT_PROCESSING(FILE* fp) {
	fclose(fp);
	return NULL;
}

BYTE* Read24BMP(int &nWidth, int &nHeight, const char *strFileName) {
	if (!strFileName)return NULL;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;
	FILE* fp;
	BYTE *pImg, *pCur;
	BYTE tmp;
	int ImgSize, Patch, Extend, i;

	if ((fp = fopen(strFileName, "rb")) == NULL)return NULL;
	if (fread((void*)&FileHeader, 1, 14, fp) != 14) { return FAULT_PROCESSING(fp); }
	if (fread((void*)&InfoHeader, 1, 40, fp) != 40) { return FAULT_PROCESSING(fp); }
	if (FileHeader.bfOffBits < 54) { return FAULT_PROCESSING(fp); }
	if (InfoHeader.biBitCount != 24) { return FAULT_PROCESSING(fp); }

	nWidth = (int)InfoHeader.biWidth;
	nHeight = (int)InfoHeader.biHeight;

	Patch = 3 * nWidth;
	ImgSize = Patch*nHeight;
	fseek(fp, FileHeader.bfOffBits, SEEK_SET);

	if ((pImg = new BYTE[ImgSize]) == NULL) { return FAULT_PROCESSING(fp); }
	
	Extend = (nWidth * 3 + 3) / 4 * 4 - nWidth * 3;
	for (pCur = pImg + ImgSize - Patch;
		pCur >= pImg;
		pCur -= Patch)
	{
		if (fread((void *)pCur, 1, Patch, fp) != UINT(Patch))
		{
			delete[]pImg; fclose(fp); return NULL;
		}
		for (i = 0; i < Extend; i++)
			if (fread(&tmp, 1, 1, fp) != 1)
			{
				delete[]pImg; fclose(fp); return NULL;
			}
	}
	fclose(fp);
	return pImg;
}

bool Write24BMP(BYTE *pImg,
	INT nWidth,
	INT nHeight,
	const char *strFileName)
{
	if (!strFileName)	return false;
	if (!pImg)			return false;

	BITMAPFILEHEADER	FileHeader;
	BITMAPINFOHEADER	InfoHeader;
	FILE				*fp;
	BYTE				*pCur=NULL;
	int					ImgSize, Patch, Extend, i;

	Patch = nWidth * 3;
	ImgSize = (nWidth + 3) / 4 * 4 * nHeight;

	if ((fp = fopen(strFileName, "wb+")) == NULL) return false;
	FileHeader.bfType = ((WORD)('M' << 8) | 'B');
	FileHeader.bfOffBits = 54; // sizeof(FileHeader)+sizeof(InfoHeader)
	FileHeader.bfSize = FileHeader.bfOffBits + ImgSize;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;

	InfoHeader.biSize = 40;
	InfoHeader.biWidth = nWidth;
	InfoHeader.biHeight = nHeight;
	InfoHeader.biPlanes = 1;
	InfoHeader.biBitCount = 24;
	InfoHeader.biCompression = 0;
	InfoHeader.biSizeImage = ImgSize;
	InfoHeader.biXPelsPerMeter = 0;
	InfoHeader.biYPelsPerMeter = 0;
	InfoHeader.biClrUsed = 0;
	InfoHeader.biClrImportant = 0;

	if (fwrite((void *)&FileHeader, 1, 14, fp) != 14)
	{
		fclose(fp); remove(strFileName); return false;
	}
	if (fwrite((void *)&InfoHeader, 1, 40, fp) != 40)
	{
		fclose(fp); remove(strFileName); return false;
	}

	Extend = (nWidth * 3 + 3) / 4 * 4 - nWidth * 3;
	for (pCur = pImg + (nHeight - 1)*Patch;
		pCur >= pImg;
		pCur -= Patch)
	{
		if (fwrite((void *)pCur, 1, Patch, fp) != UINT(Patch))
		{
			fclose(fp); remove(strFileName); return false;
		}

		for (i = 0; i < Extend; i++)
			if (fwrite((void *)(pCur + Patch - 3), 1, 1, fp) != 1)
			{
				fclose(fp); remove(strFileName); return false;
			}

	}
	fclose(fp);
	return true;
}

BYTE*** Flow_Map(BYTE* pImg, int nWidth, int nHeight) {
	BYTE*** pMap=new BYTE**[3];
	for (int i = 0; i < 3; i++) {
		pMap[i] = new BYTE*[nHeight];
		for (int j = 0; j < nHeight; j++) {
			pMap[i][j] = new BYTE[nWidth];
		}
	}
	int i = 0;
	BYTE* begin = pImg;
	while (i < 3 * nWidth*nHeight) {
		pMap[0][i / 3 / nWidth][i / 3 % nWidth] = *begin;
		pMap[1][i / 3 / nWidth][i / 3 % nWidth] = *(begin+1);
		pMap[2][i / 3 / nWidth][i / 3 % nWidth] = *(begin+2);
		begin += 3;
		i+=3;
	}
	return pMap;
}

BYTE *Map_Flow(BYTE***pMap, int nWidth, int nHeight) {
	BYTE* pImg = new BYTE[3 * nWidth*nHeight];
	BYTE* begin = pImg;
	int i = 0;
	while (i < 3 * nWidth*nHeight) {
		*begin = pMap[0][i / 3 / nWidth][i / 3 % nWidth];
		*(begin+1) = pMap[1][i / 3 / nWidth][i / 3 % nWidth];
		*(begin+2) = pMap[2][i / 3 / nWidth][i / 3 % nWidth];
	}
	return pImg;
}

BYTE* over_scale_avoid_add(BYTE*origin, int*add) {
	int int_origin = *origin;
	int_origin += *add;
	if (int_origin < 0)int_origin = 0;
	else if (int_origin > 255)int_origin = 255;
	*origin = BYTE(int_origin);
	return origin;
}

BYTE multiply_float(BYTE*origin, float*degree) {

	float r,g,b,new_val;
	r = *origin**degree;
	g = *(origin+1)**(degree+1);
	b = *(origin+2)**(degree+2);
	new_val = r + g + b;
	if (new_val > 255.0)new_val = 255.0;
	else if (new_val < 0.0)new_val = 0.0;
	BYTE _new_val = BYTE(new_val);
	return _new_val;
}

BYTE* Color_Change(int*degree, BYTE* pImg, int nWidth,int nHeight) {
	BYTE* begin = pImg;
	int i = 0;
	while (i<nWidth*3*nHeight) {
		over_scale_avoid_add(begin, degree);
		*begin++;
		over_scale_avoid_add(begin, degree+1);
		*begin++;
		over_scale_avoid_add(begin, degree+2);
		*begin++;
		i += 3;
	}
	return pImg;
}

BYTE* RGB_YUV(BYTE*pImg,INT nWidth,INT nHeight) {
	float Y[3] = { 0.3,0.59,0.11 };
	float U[3] = { 0.7,-0.59,-0.11 };
	float V[3] = { -0.3,-0.59,0.89 };

	BYTE* yuv_pImg = new BYTE[3 * nWidth*nHeight];
	BYTE* begin = pImg;
	BYTE* new_begin = yuv_pImg;
	int i = 0;
	while (i<3*nWidth*nHeight) {
		*new_begin = multiply_float(begin, Y);
		new_begin++;
		*new_begin = multiply_float(begin, U);
		new_begin++;
		*new_begin = multiply_float(begin, V);
		new_begin++;
		begin += 3;
		i += 3;
	}
	return yuv_pImg;
}

int avoid_overscale(int i) {
	if (i > 255)i = 255;
	else if (i < 0)i = 0;
	return i;
}

BYTE *YUV_RGB(BYTE*pImg,INT nWidth,INT nHeight) {

	BYTE*rgb_pImg = new BYTE[3 * nWidth*nHeight];
	BYTE* begin = pImg;
	BYTE* new_begin = rgb_pImg;
	int i = 0;
	int y, u, v,r,g,b;
	while (i<3*nWidth*nHeight) {
		y = *begin,u=*(begin+1),v=*(begin+2);
		r = y + u;
		*new_begin = avoid_overscale(r);
		new_begin++;
		b = y + v;
		float temp;
		temp = (v + (float)r*0.3 - (float)b*0.89) / (-0.59);
		g = int(temp);
		
		*new_begin = avoid_overscale(g);
		new_begin++;
		*new_begin = avoid_overscale(b);
		new_begin++;

		begin += 3;
		i += 3;
	}
	return rgb_pImg;
}

BYTE* Constrast_Chage(float constrast_ratio, BYTE* pImg,INT nWidth,INT nHeight) {
	BYTE* constrast_pImg = new BYTE[3 * nWidth*nHeight];
	int max_Y=0, min_Y=255;
	int i = 0;
	BYTE* begin = pImg,*new_begin=constrast_pImg;
	while (i < 3 * nWidth*nHeight) {
		*new_begin = avoid_overscale(int(float(*begin - 127.0)*constrast_ratio + 127.0));
		*(new_begin + 1) = *(begin + 1);
		*(new_begin + 2) = *(begin + 2);
		begin += 3, new_begin += 3, i += 3;
	}
	return constrast_pImg;
}

