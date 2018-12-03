#include"laplace.h"

BYTE* laplace_op(BYTE*pImg, INT nWidth, INT nHeight) {
	BYTE* filtered_pImg = new BYTE[nWidth*nHeight * 3];
	BYTE*** pMap = Flow_Map(pImg, nWidth, nHeight);
	BYTE* begin = filtered_pImg;

	for (int i = 0; i < nHeight; i++) {
		if (i == 0 || i == nHeight - 1) {
			for (int k = 0; k < nWidth;k++) {
				*begin = pMap[0][i][k];
				*begin++;
				*begin = pMap[1][i][k];
				*begin++;
				*begin = pMap[2][i][k];
				*begin++;
			}
			continue;
		}
		for (int j = 0; j < nWidth; j++) {
			if (j == 0 || j == nWidth - 1) {
				*begin = pMap[0][i][j];
				*begin++;
				*begin = pMap[1][i][j];
				*begin++;
				*begin = pMap[2][i][j];
				*begin++;
			}
			else {
				*begin = -((pMap[0][i + 1][j] + pMap[0][i - 1][j] + pMap[0][i][j + 1] + pMap[0][i][j - 1]) - 4 * pMap[0][i][j])+ pMap[0][i][j];
				begin++;
				*begin = -((pMap[1][i + 1][j] + pMap[1][i - 1][j] + pMap[1][i][j + 1] + pMap[1][i][j - 1]) - 4 * pMap[1][i][j])+ pMap[1][i][j];
				begin++;
				*begin = -((pMap[2][i + 1][j] + pMap[2][i - 1][j] + pMap[2][i][j + 1] + pMap[2][i][j - 1]) - 4 * pMap[2][i][j])+ pMap[2][i][j];
				begin++;
			}
		}
	}
	return filtered_pImg;
}