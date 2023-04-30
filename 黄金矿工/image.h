#include <graphics.h>
#include <conio.h>
#pragma once

//该头文件包含图像相关功能函数


//左右翻转图片
void FlipImage(IMAGE* pDst, IMAGE* pSrc) {
	DWORD* pdSrc = GetImageBuffer(pSrc);
	DWORD* pdDst = GetImageBuffer(pDst);
	int width = pSrc->getwidth();
	int height = pSrc->getheight();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int r = GetRValue(pdSrc[x + y * width]);
			int g = GetGValue(pdSrc[x + y * width]);
			int b = GetBValue(pdSrc[x + y * width]);
			pdDst[width - x + y * width] = RGB(r, g, b);
		}
	}
}


//利用蒙版图层打印透明背景图片
void PutImageWithMask(int PosX, int PosY, IMAGE* pImg, IMAGE* pImgMask) {
	putimage(PosX, PosY, pImgMask, NOTSRCERASE);
	putimage(PosX, PosY, pImg, SRCINVERT);
}


//打印透明背景png图
void TransparentImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}


//缩放图片
IMAGE ZoomImage(IMAGE* Q, double ZoomRate, bool HighQuality = false, double ZoomRate2 = 0)
{
	//不填写第二缩放参数则默认和第一相等
	if (ZoomRate2 == 0)
		ZoomRate2 = ZoomRate;

	IMAGE tmp;
	tmp = *Q;
	IMAGE P = tmp;

	//根据缩放比率设定目标图像大小
	P.Resize((int)(Q->getwidth() * ZoomRate), (int)(Q->getheight() * ZoomRate2));

	//分别对原图像和目标图像获取指针
	DWORD* M = GetImageBuffer(&P);
	DWORD* N = GetImageBuffer(Q);

	//选择高质量则使用双线性插值算法
	if (HighQuality)
	{
		for (int i = 0; i < P.getheight(); i++)
		{
			for (int j = 0; j < P.getwidth(); j++)
			{
				//求出目标图像对应像素点在原图的浮点坐标并取整
				int X_ = (int)((j + 0.5) / ZoomRate - 0.5);
				int Y_ = (int)((i + 0.5) / ZoomRate2 - 0.5);

				//根据取整坐标求A1(X,Y), A2(X+1,Y), A3(X,Y+1), A4(X+1,Y+1)即浮点坐标临近4个点的颜色平均值。
				M[j + i * P.getwidth()] = RGB((GetRValue(N[X_ + Y_ * Q->getwidth()]) + GetRValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetRValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetRValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4,
					(GetGValue(N[X_ + Y_ * Q->getwidth()]) + GetGValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetGValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetGValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4,
					(GetBValue(N[X_ + Y_ * Q->getwidth()]) + GetBValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetBValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetBValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4);
			}
		}
	}
	else
		//选择低质量则按常规方法缩放
	{
		for (int i = 0; i < P.getheight(); i++)
			for (int j = 0; j < P.getwidth(); j++)
				//根据目标图像像素点位置逆推算原图像像素点赋值
				M[j + i * P.getwidth()] = N[(int)(j / ZoomRate) + (int)(i / ZoomRate2) * Q->getwidth()];
	}

	return P;
}