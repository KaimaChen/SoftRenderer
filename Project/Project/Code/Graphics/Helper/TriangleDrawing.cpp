#include "TriangleDrawing.h"

//《3D游戏编程大师技巧》介绍的光栅化方法：将三角形分成平顶和平底两种三角形进行绘制
void TriangleDrawing::T3DDrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Callback drawPixel)
{
	Sort(x0, y0, x1, y1, x2, y2);
	
	if (y0 == y1) //平底
	{
		DrawTriangleBottom(x2, y2, x1, y1, x0, y0, drawPixel);
	}
	else if (y1 == y2) //平顶
	{
		DrawTriangleTop(x1, y1, x2, y2, x0, y0, drawPixel);
	}
	else //切分三角形s
	{
		float fx = (float)(y1 - y0) / (y2 - y0) * (x2 - x0) + x0;
		int ix = (int)(fx + 0.5f);

		//Right Side Major: P1在P2左边
		//		  p2
		//      / \
		//    /     \
		//p1         \
		//				  \
		//                p0
		if (x1 < x2) 
		{
			if (x1 > ix) Tools::Swap<int>(x1, ix); //保证p1在新点左边，因为p0可能比p1左而导致新点在p1左边生成

			DrawTriangleTop(x1, y1, ix, y1, x0, y0, drawPixel);
			DrawTriangleBottom(x2, y2, ix, y1, x1, y1, drawPixel);
		}
		//Left Side Major: P1在P2右边
		//           p2
		//         /    \
		//       /        \
		//     /            p1
		//   /
		//p0
		else 
		{
			if (x1 < ix) Tools::Swap<int>(x1, ix); //保证p1在新点右边

			DrawTriangleTop(ix, y1, x1, y1, x0, y0, drawPixel);
			DrawTriangleBottom(x2, y2, x1, y1, ix, y1, drawPixel);
		}
	}
}

//先在y方向上从小到大排，对于y相同的在x方向上从小到大排序
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

//绘制平顶三角形，传入的三个点必须是排序好了的，如下：
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

//绘制平底三角形，传入的三个点必须是排序好了的，如下：
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