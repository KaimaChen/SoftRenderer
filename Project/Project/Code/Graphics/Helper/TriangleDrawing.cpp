#include "TriangleDrawing.h"

//��3D��Ϸ��̴�ʦ���ɡ����ܵĹ�դ���������������ηֳ�ƽ����ƽ�����������ν��л���
void TriangleDrawing::T3DDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel)
{
	Sort(x0, y0, x1, y1, x2, y2);
	
	if (y0 == y1) //ƽ��
	{
		DrawTriangleBottom(x2, y2, x1, y1, x0, y0, drawPixel);
	}
	else if (y1 == y2) //ƽ��
	{
		DrawTriangleTop(x1, y1, x2, y2, x0, y0, drawPixel);
	}
	else //�з�������s
	{
		float fx = (float)(y1 - y0) / (y2 - y0) * (x2 - x0) + x0;
		int ix = (int)(fx + 0.5f);

		//Right Side Major: P1��P2���
		//		  p2
		//      / \
		//    /     \
		//p1         \
		//				  \
		//                p0
		if (x1 < x2) 
		{
			if (x1 > ix) Tools::Swap<int>(x1, ix); //��֤p1���µ���ߣ���Ϊp0���ܱ�p1��������µ���p1�������

			DrawTriangleTop(x1, y1, ix, y1, x0, y0, drawPixel);
			DrawTriangleBottom(x2, y2, ix, y1, x1, y1, drawPixel);
		}
		//Left Side Major: P1��P2�ұ�
		//           p2
		//         /    \
		//       /        \
		//     /            p1
		//   /
		//p0
		else 
		{
			if (x1 < ix) Tools::Swap<int>(x1, ix); //��֤p1���µ��ұ�

			DrawTriangleTop(ix, y1, x1, y1, x0, y0, drawPixel);
			DrawTriangleBottom(x2, y2, x1, y1, ix, y1, drawPixel);
		}
	}
}

//����y�����ϴ�С�����ţ�����y��ͬ����x�����ϴ�С��������
void TriangleDrawing::Sort(int &x0, int &y0, int &x1, int &y1, int &x2, int &y2)
{
	if (y2 < y1)
	{
		Tools::Swap<int>(y1, y2);
		Tools::Swap<int>(x1, x2);
	}
	if (y1 < y0)
	{
		Tools::Swap<int>(y0, y1);
		Tools::Swap<int>(x0, x1);
	}
	if (y2 < y1)
	{
		Tools::Swap<int>(y1, y2);
		Tools::Swap<int>(x1, x2);
	}

	if (y0 == y1 && x1 < x0)
		Tools::Swap<int>(x0, x1);
	if (y1 == y2 && x2 < x1)
		Tools::Swap<int>(x1, x2);
}

void TriangleDrawing::DrawLine(int xs, int xe, int y, Callback drawPixel)
{
	for (int x = xs; x <= xe; ++x)
	{
		drawPixel(x, y, 0, Color::white);
	}
}

//����ƽ�������Σ�����������������������˵ģ����£�
//p0-------p1
//   \       /
//     \   /
//       p2
void TriangleDrawing::DrawTriangleTop(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel)
{
	if (y0 != y1)
		return;

	float leftDxy = (float)(x2 - x0) / (y2 - y0);
	float rightDxy = (float)(x2 - x1) / (y2 - y1);

	float xs = x0;
	float xe = x1;

	for (int y = y0; y >= y2; --y)
	{
		DrawLine((int)(xs + 0.5f), (int)(xe + 0.5f), y, drawPixel);
		xs -= leftDxy;
		xe -= rightDxy;
	}
}

//����ƽ�������Σ�����������������������˵ģ����£�
//       p0
//     /    \
//   /        \
//p2-------p1
void TriangleDrawing::DrawTriangleBottom(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel)
{
	if (y2 != y1)
		return;

	float leftDxy = (float)(x2 - x0) / (y2 - y0);
	float rightDxy = (float)(x1 - x0) / (y1 - y0);

	float xs = x2;
	float xe = x1;

	for (int y = y2; y <= y0; ++y)
	{
		DrawLine((int)(xs + 0.5f), (int)(xe + 0.5f), y, drawPixel);
		xs += leftDxy;
		xe += rightDxy;
	}
}