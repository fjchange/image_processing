#include"filter.h"
#include<cmath>
#include<vector>
#include<algorithm>

using namespace std;

BYTE* mean_filter(BYTE*pImg, INT range,INT nWidth, INT nHeight) {
	BYTE* filtered_pImg = new BYTE[nWidth*nHeight * 3];
	BYTE*** pMap = Flow_Map(pImg, nWidth, nHeight);
	BYTE* begin = filtered_pImg;
	int temp[3] = { 0 };
	int count = 0;
	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			for (int k = -range; k < range; k++) {
				if (i + k<0 || i + k>nHeight - 1)continue;
				for (int m = -range; m < range; m++) {
					if (j + m<0 || j + m>nWidth - 1)continue;
					temp[0] += pMap[0][i + k][j + m];
					temp[1] += pMap[2][i + k][j + m];
					temp[2] += pMap[1][i + k][j + m];
					count++;
				}
			}
			*begin = temp[0] / count;
			*begin++;
			*begin = temp[1] / count;
			*begin++;
			*begin = temp[2] / count;
			*begin++;
			count = 0;
			temp[0] = 0, temp[1] = 0, temp[2] = 0;
		}
	}
	return filtered_pImg;
}

BYTE* center_filter(BYTE*pImg,INT range, INT nWidth, INT nHeight) {
	BYTE* filtered_pImg = new BYTE[nWidth*nHeight * 3];
	BYTE*** pMap = Flow_Map(pImg, nWidth, nHeight);
	BYTE* begin = filtered_pImg;
	vector<int> v0,v1,v2;
	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			for (int k = -range; k < range; k++) {
				if (i + k<0 || i + k>nHeight - 1)continue;
				for (int m = -range; m < range; m++) {
					if (j + m<0 || j + m>nWidth - 1)continue;
					v0.push_back(pMap[0][i + k][j + m]);
					v1.push_back(pMap[1][i + k][j + m]);
					v2.push_back(pMap[2][i + k][j + m]);
				}
			}
			sort(v0.begin(), v0.end());
			sort(v1.begin(), v1.end());
			sort(v2.begin(), v2.end());

			*begin = v0[v0.size() / 2];
			*begin++;
			*begin = v1[v0.size() / 2];
			*begin++;
			*begin = v2[v0.size() / 2];
			*begin++;
			v0.clear(), v1.clear(), v2.clear();
		}
	}
	return filtered_pImg;
}