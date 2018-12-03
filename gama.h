#pragma once
#include<Windows.h>
#include<stdio.h>

BYTE* inverse(BYTE*pImg, INT nWidth, INT nHeight);
BYTE* gray_inverse(BYTE*pImg, INT nWidth, INT nHeight);
BYTE* gama_trans(BYTE*pImg, float gama, int nWidth, int nHeight);
