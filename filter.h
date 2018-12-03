#pragma once
#include<Windows.h>
#include<stdio.h>
#include"BMP.h"

BYTE* mean_filter(BYTE*pImg, INT range, INT nWidth, INT nHeight);
BYTE* center_filter(BYTE*pImg, INT range, INT nWidth, INT nHeight);