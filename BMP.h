#pragma once
#include<Windows.h>
#include<stdio.h>

//stop the fstream when fault happens
BYTE* FAULT_PROCESSING(FILE* fp);

//BMP read and write
BYTE* Read24BMP(int &nWidth, int &nHeight, const char *strFileName);
bool Write24BMP(BYTE *pImg,INT nWidth,INT nHeight,const char *strFileName);

//color change ,int degree is int[3] for each channel
BYTE* Color_Change(int*degree, BYTE* pImg, int nWidth, int nHeight);
//constrast change, constrast_ratio 1.0 means no change
BYTE* Constrast_Chage(float constrast_ratio, BYTE* pImg, INT nWidth, INT nHeight);

BYTE *YUV_RGB(BYTE*pImg, INT nWidth, INT nHeight);
BYTE* RGB_YUV(BYTE*pImg, INT nWidth, INT nHeight);

BYTE*** Flow_Map(BYTE* pImg, int nWidth, int nHeight);
BYTE *Map_Flow(BYTE***pMap, int nWidth, int nHeight);

//some function for the functions above
int avoid_overscale(int i);
BYTE* over_scale_avoid_add(BYTE*origin, int*add);
BYTE multiply_float(BYTE*origin, float*degree);