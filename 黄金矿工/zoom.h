#include <graphics.h>
#include <conio.h>
#pragma once

IMAGE ZoomImage(IMAGE* Q, double ZoomRate, bool HighQuality = false, double ZoomRate2 = 0)
{
	//����д�ڶ����Ų�����Ĭ�Ϻ͵�һ���
	if (ZoomRate2 == 0)
		ZoomRate2 = ZoomRate;

	IMAGE tmp;
	tmp = *Q;
	IMAGE P = tmp;

	//�������ű����趨Ŀ��ͼ���С
	P.Resize((int)(Q->getwidth() * ZoomRate), (int)(Q->getheight() * ZoomRate2));

	//�ֱ��ԭͼ���Ŀ��ͼ���ȡָ��
	DWORD* M = GetImageBuffer(&P);
	DWORD* N = GetImageBuffer(Q);

	//ѡ���������ʹ��˫���Բ�ֵ�㷨
	if (HighQuality)
	{
		for (int i = 0; i < P.getheight(); i++)
		{
			for (int j = 0; j < P.getwidth(); j++)
			{
				//���Ŀ��ͼ���Ӧ���ص���ԭͼ�ĸ������겢ȡ��
				int X_ = (int)((j + 0.5) / ZoomRate - 0.5);
				int Y_ = (int)((i + 0.5) / ZoomRate2 - 0.5);

				//����ȡ��������A1(X,Y), A2(X+1,Y), A3(X,Y+1), A4(X+1,Y+1)�����������ٽ�4�������ɫƽ��ֵ��
				M[j + i * P.getwidth()] = RGB((GetRValue(N[X_ + Y_ * Q->getwidth()]) + GetRValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetRValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetRValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4,
					(GetGValue(N[X_ + Y_ * Q->getwidth()]) + GetGValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetGValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetGValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4,
					(GetBValue(N[X_ + Y_ * Q->getwidth()]) + GetBValue(N[(X_ + 1) + Y_ * Q->getwidth()]) + GetBValue(N[X_ + (Y_ + 1) * Q->getwidth()]) + GetBValue(N[(X_ + 1) + (Y_ + 1) * Q->getwidth()])) / 4);
			}
		}
	}
	else
		//ѡ��������򰴳��淽������
	{
		for (int i = 0; i < P.getheight(); i++)
			for (int j = 0; j < P.getwidth(); j++)
				//����Ŀ��ͼ�����ص�λ��������ԭͼ�����ص㸳ֵ
				M[j + i * P.getwidth()] = N[(int)(j / ZoomRate) + (int)(i / ZoomRate2) * Q->getwidth()];
	}

	return P;
}