#pragma once
#include <math.h>
#include "framework.h"
class VertexUIColorStandard
{
public:
	//42 47 56
	//38 41 54
	//241 242 244
	class titlebar
	{
	public:
		//158 142 250
		int oR = 158;
		int oG = 142;
		int oB = 250;
		int bg_r = 158;
		int bg_g = 142;
		int bg_b = 250;
		int bg_d1_r, bg_d1_g, bg_d1_b = 10;
		int bg_d2_r, bg_d2_g, bg_d2_b = 30;
	};
	titlebar title;
	int bg_r = 255;
	int bg_g = 255;
	int bg_b = 255;
	int bg_d1_r, bg_d1_g, bg_d1_b = 240;
	int bg_d2_r, bg_d2_g, bg_d2_b = 220;

	int ctl_d1_r, ctl_d1_g, ctl_d1_b;
	int ctl_blur_r, ctl_blur_g, ctl_blur_b;

	int txt_r, txt_g, txt_b;
};
VertexUIColorStandard vuicolor;

int GetMaxValue(int num, int max)
{
	if (num >= max)return max;
	else return num;
}
int GetMinValue(int num, int min)
{
	if (num <= min)return min;
	else return num;
}
float GetMaxValue2(float num, float max)
{
	if (num >= max)return max;
	else return num;
}
float GetMinValue2(float num, float min)
{
	if (num <= min)return min;
	else return num;
}
int SetColorBright(int c,int b,bool DarkModeNotChange=false)
{
	if (c > 128) { if (c - b >= 255)return 255; else return c - b; }
	else if (DarkModeNotChange == false)return c + b;
	else return c;
}

int SetThemedColorBright(int c, int b, int Themed,bool DarkModeNotChange = false)
{
	if (c > 128) { return Themed; }
	else if (DarkModeNotChange == false)return c + b;
	else return c;
}

int VuiBlackOrWhiteOpposite(int c)
{
	if (c > 128)return 20;
	else return 244;
}

int VuiBlackOrWhiteMedium(int c)
{
	if (c > 128)return 128;
	else return 15;
}
unsigned long VuiColorAverage(unsigned long Clr)
{
	return (GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3;
}
unsigned long VuiMediumChangeToLight(unsigned long Clr,int Val=30)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)return Clr;
	else return RGB(GetMaxValue(GetRValue(Clr)+Val,255), GetMaxValue(GetGValue(Clr)+Val, 255), GetMaxValue(GetBValue(Clr)+Val, 255));
}
unsigned long VuiDarkChangeToMedium(unsigned long Clr, int Val = 30)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) >= 128)return Clr;
	return RGB(GetMaxValue(GetRValue(Clr) + Val, 255), GetMaxValue(GetGValue(Clr) + Val, 255), GetMaxValue(GetBValue(Clr) + Val, 255));
}
unsigned long VuiCalcShadow(unsigned long Clr)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)return RGB(25, 25, 27);
	else
		if (((GetRValue(Clr) < GetBValue(Clr)-8) && GetGValue(Clr) < GetBValue(Clr)-8))return RGB(GetRValue(Clr) - 40, GetGValue(Clr) - 40, GetBValue(Clr) - 20);
		else if (((GetGValue(Clr) < GetRValue(Clr) - 8) && GetBValue(Clr) < GetRValue(Clr) - 8))return RGB(GetRValue(Clr) - 20, GetGValue(Clr) - 40, GetBValue(Clr) - 40);
		else if (((GetRValue(Clr) < GetGValue(Clr) - 8) && GetBValue(Clr) < GetGValue(Clr) - 5))return RGB(GetRValue(Clr) - 40, GetGValue(Clr) - 20, GetBValue(Clr) - 40);
		else return RGB(190, 190, 194);
}
unsigned long VuiAddColorValue(unsigned long Clr, int r, int g, int b)
{
	int nR, nG, nB;
	nR = GetMaxValue(GetRValue(Clr)+r,255);
	nG = GetMaxValue(GetGValue(Clr)+g,255);
	nB = GetMaxValue(GetBValue(Clr)+b,255);
	return RGB(nR, nG, nB);
}

unsigned long VuiFadeColor(unsigned long Clr, int val)
{
	int nR, nG, nB;
	if (VuiColorAverage(Clr) > 128)val = -val;
	else val = val;
	nR = GetMaxValue(GetRValue(Clr) + val, 255);
	nG = GetMaxValue(GetGValue(Clr) + val, 255);
	nB = GetMaxValue(GetBValue(Clr) + val, 255);
	return RGB(nR, nG, nB);
}

unsigned long VuiFadeColor2(unsigned long Clr, int val) //适配
{
	int nR, nG, nB;
	if (VuiColorAverage(Clr) > 128)val = -val*3;
	else val = val;
	nR = GetMaxValue(GetRValue(Clr) + val, 255);
	nG = GetMaxValue(GetGValue(Clr) + val, 255);
	nB = GetMaxValue(GetBValue(Clr) + val, 255);
	return RGB(nR, nG, nB);
}

unsigned long VuiDarkenColor(unsigned long Clr, int val)
{
	int nR, nG, nB;
	nR = GetMinValue(GetRValue(Clr) - val, 0);
	nG = GetMinValue(GetGValue(Clr) - val, 0);
	nB = GetMinValue(GetBValue(Clr) - val, 0);
	return RGB(nR, nG, nB);
}

unsigned long VuiSetSaturate(unsigned long Clr, int val)
{
	// 1. 获取原始RGB分量 (0.0 - 1.0)
	double r = GetRValue(Clr) / 255.0;
	double g = GetGValue(Clr) / 255.0;
	double b = GetBValue(Clr) / 255.0;

	// 2. 转换到HSV色彩空间
	// Find max and min using conditional operators
	double max_val = (r > g ? (r > b ? r : b) : (g > b ? g : b));
	double min_val = (r < g ? (r < b ? r : b) : (g < b ? g : b));
	double delta = max_val - min_val;

	double h = 0.0; // Hue
	double s = 0.0; // Saturation
	double v = max_val; // Value (Brightness)

	// 计算饱和度 (S)
	if (max_val != 0.0) {
		s = delta / max_val;
	}

	// 计算色相 (H)
	if (delta != 0.0) {
		if (max_val == r) {
			h = (g - b) / delta;
			if (h < 0) h += 6.0;
		}
		else if (max_val == g) {
			h = (b - r) / delta + 2.0;
		}
		else { // max_val == b
			h = (r - g) / delta + 4.0;
		}
		h /= 6.0;
	}

	// 3. 调整饱和度
	double saturation_factor = 1.0 + val / 100.0;
	s *= saturation_factor;

	// 饱和度限制在 [0, 1] 范围内 using conditional operator
	s = (s < 0.0 ? 0.0 : (s > 1.0 ? 1.0 : s));

	// 4. 将调整后的HSV值转换回RGB
	double c = v * s; // Chroma
	// Use conditional operator for abs
	double x = c * (1.0 - (fabs(fmod(h * 6.0, 2.0)) - 1.0));
	// If you want to avoid <cmath> entirely, uncomment the lines below and comment the line above
	/*
	double fmod_result = my_fmod(h * 6.0, 2.0);
	double abs_fmod_result = my_abs(fmod_result);
	double x = c * (1.0 - (abs_fmod_result - 1.0));
	*/

	double m = v - c;

	double rp, gp, bp;
	if (0 <= h && h < 1) { rp = c; gp = x; bp = 0; }
	else if (1 <= h && h < 2) { rp = x; gp = c; bp = 0; }
	else if (2 <= h && h < 3) { rp = 0; gp = c; bp = x; }
	else if (3 <= h && h < 4) { rp = 0; gp = x; bp = c; }
	else if (4 <= h && h < 5) { rp = x; gp = 0; bp = c; }
	else { rp = c; gp = 0; bp = x; } // 5 <= h < 6

	// Use GetMaxValue which uses conditional operators
	int nR = GetMaxValue(static_cast<int>(round((rp + m) * 255)), 255);
	int nG = GetMaxValue(static_cast<int>(round((gp + m) * 255)), 255);
	int nB = GetMaxValue(static_cast<int>(round((bp + m) * 255)), 255);

	return RGB(nR, nG, nB);
}
unsigned long VuiNewSetSaturate(unsigned long Clr, int val)
{
	// 1. 获取原始RGB分量 (0.0 - 1.0)
	double r = GetRValue(Clr) / 255.0;
	double g = GetGValue(Clr) / 255.0;
	double b = GetBValue(Clr) / 255.0;

	// 2. 转换到HSV色彩空间（我们用 HSV，因为更直观）
	double max_val = (r > g ? (r > b ? r : b) : (g > b ? g : b));
	double min_val = (r < g ? (r < b ? r : b) : (g < b ? g : b));
	double delta = max_val - min_val;

	double h = 0.0; // Hue
	double s = 0.0; // Saturation
	double v = max_val; // Value (Brightness)

	// 计算饱和度 (S)
	if (max_val != 0.0) {
		s = delta / max_val;
	}

	// 计算色相 (H)
	if (delta != 0.0) {
		if (max_val == r) {
			h = (g - b) / delta;
			if (h < 0) h += 6.0;
		}
		else if (max_val == g) {
			h = (b - r) / delta + 2.0;
		}
		else { // max_val == b
			h = (r - g) / delta + 4.0;
		}
		// h 现在是 0~6 的值，不要除以6！
	}

	// 3. 调整饱和度
	double saturation_factor = 1.0 + val / 100.0;
	s *= saturation_factor;

	// 饱和度限制在 [0, 1] 范围内
	s = (s < 0.0 ? 0.0 : (s > 1.0 ? 1.0 : s));

	// 4. 将调整后的HSV值转换回RGB
	// 使用标准的 HSV -> RGB 转换
	double c = v * s; // Chroma
	double h_prime = h; // 保持 0~6
	double x = c * (1.0 - fabs(fmod(h_prime, 2.0) - 1.0)); // 注意：这里用 fmod 和 abs
	double m = v - c;

	double rp, gp, bp;
	if (h_prime < 1.0) { rp = c; gp = x; bp = 0; }
	else if (h_prime < 2.0) { rp = x; gp = c; bp = 0; }
	else if (h_prime < 3.0) { rp = 0; gp = c; bp = x; }
	else if (h_prime < 4.0) { rp = 0; gp = x; bp = c; }
	else if (h_prime < 5.0) { rp = x; gp = 0; bp = c; }
	else { rp = c; gp = 0; bp = x; } // 5 <= h_prime < 6

	// 转回 0~255
	int nR = GetMaxValue(static_cast<int>((rp + m) * 255), 255);
	int nG = GetMaxValue(static_cast<int>((gp + m) * 255), 255);
	int nB = GetMaxValue(static_cast<int>((bp + m) * 255), 255);

	return RGB(nR, nG, nB);
}
void RGBtoHSL(int r, int g, int b, double& h, double& s, double& l) {
	double red = r / 255.0;
	double green = g / 255.0;
	double blue = b / 255.0;

	// 找出最大值和最小值
	double maxVal = red, minVal = red;
	if (green > maxVal) maxVal = green;
	if (blue > maxVal) maxVal = blue;
	if (green < minVal) minVal = green;
	if (blue < minVal) minVal = blue;

	double delta = maxVal - minVal;

	// 计算亮度
	l = (maxVal + minVal) * 0.5;

	if (delta < 0.00001) {
		h = s = 0.0;
		return;
	}

	// 计算饱和度
	if (l > 0.5) {
		s = delta / (2.0 - maxVal - minVal);
	}
	else {
		s = delta / (maxVal + minVal);
	}

	// 计算色相
	if (maxVal == red) {
		h = (green - blue) / delta;
		if (h < 0.0) h += 6.0;
	}
	else if (maxVal == green) {
		h = (blue - red) / delta + 2.0;
	}
	else {
		h = (red - green) / delta + 4.0;
	}

	h *= 60.0;
	if (h < 0.0) h += 360.0;
}
unsigned long HSLtoRGB(double h, double s, double l)
{
	if (s == 0.0)
	{
		int c = static_cast<int>(l * 255.0 + 0.5);
		return RGB(c, c, c);
	}

	double q = (l < 0.5) ? l * (1.0 + s) : l + s - l * s;
	double p = 2.0 * l - q;

	auto hueToRGB = [&](double t) -> double {
		if (t < 0.0) t += 1.0;
		if (t > 1.0) t -= 1.0;
		if (t * 6.0 < 1.0) return p + (q - p) * 6.0 * t;
		if (t * 2.0 < 1.0) return q;
		if (t * 3.0 < 2.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
		return p;
	};

	double r = hueToRGB(h / 360.0 + 1.0 / 3.0);
	double g = hueToRGB(h / 360.0);
	double b = hueToRGB(h / 360.0 - 1.0 / 3.0);

	int ir = GetMaxValue(static_cast<int>(r * 255.0 + 0.5), 255);
	int ig = GetMaxValue(static_cast<int>(g * 255.0 + 0.5), 255);
	int ib = GetMaxValue(static_cast<int>(b * 255.0 + 0.5), 255);

	return RGB(ir, ig, ib);
}
unsigned long VuiAdaptColor(unsigned long Clr)
{
	int r = GetRValue(Clr);
	int g = GetGValue(Clr);
	int b = GetBValue(Clr);

	// 判断是否为“近似灰度色”
	if (abs(r - g) < 10 && abs(g - b) < 10 && abs(b - r) < 10)
	{
		return Clr;
	}

	// Step 1: RGB -> HSL，用于判断色相
	double h, s, l;
	RGBtoHSL(r, g, b, h, s, l);

	// Step 2: 判断是否为“绿色”（Hue 在 80° ~ 120° 之间）
	bool is_green = (h >= 70.0 && h <= 120.0);
	bool is_blue = (h >= 200 && h <= 220);
	bool is_red = (h >= 0 && h <= 30);
	if (is_green)
	{
		unsigned long adapted = VuiNewSetSaturate(Clr, 50);
		adapted = VuiDarkenColor(adapted, 10);
		int ar = GetRValue(adapted), ag = GetGValue(adapted), ab = GetBValue(adapted);
		if (ar < 40 && ag < 40 && ab < 40)
			adapted = VuiAddColorValue(adapted, 10, 10, 10);
		return adapted;
	}
	if (is_blue)
	{
		unsigned long adapted = VuiNewSetSaturate(Clr, 50);
		adapted = VuiDarkenColor(adapted, 10);
		int ar = GetRValue(adapted), ag = GetGValue(adapted), ab = GetBValue(adapted);
		if (ar < 40 && ag < 40 && ab < 40)
			adapted = VuiAddColorValue(adapted, 10, 10, 10);
		return adapted;
	}
	if (is_red)
	{
		unsigned long adapted = VuiNewSetSaturate(Clr, 30);
		adapted = VuiDarkenColor(adapted, 10);
		int ar = GetRValue(adapted), ag = GetGValue(adapted), ab = GetBValue(adapted);
		if (ar < 40 && ag < 40 && ab < 40)
			adapted = VuiAddColorValue(adapted, 10, 10, 10);
		return adapted;
	}
	else
	{
		// 生成同色系深色
		unsigned long adapted = VuiSetSaturate(Clr, -5);
		adapted = VuiDarkenColor(adapted, 40);
		int ar = GetRValue(adapted), ag = GetGValue(adapted), ab = GetBValue(adapted);
		if (ar < 40 && ag < 40 && ab < 40)
			adapted = VuiAddColorValue(adapted, 10, 10, 10);
		return adapted;
	}
}
int VuiAOrBOrCMedium(int c,int a,int b,int c1,bool DNC=false)
{
	if (c > 128)
	{
		if (c > 200)return c1;
		else return a;
	}
	else return b;
}

unsigned long AdjustBrightness(unsigned long color, float factor) {
	unsigned char r = (color >> 16) & 0xFF;
	unsigned char g = (color >> 8) & 0xFF;
	unsigned char b = color & 0xFF;
	r = min(255, (int)(r * factor));
	g = min(255, (int)(g * factor));
	b = min(255, (int)(b * factor));
	return (r << 16) | (g << 8) | b;
}

bool GetBitColor(BYTE* byte,  int x, int y, unsigned int& iRGB)
{ 
	int nPixelSize = 4;
	unsigned int* iBtRGB;

	iBtRGB = (unsigned int*)(byte + (y  * nPixelSize + x * nPixelSize));
	iRGB = RGB(GetBValue(iBtRGB[0]), GetGValue(iBtRGB[0]), GetRValue(iBtRGB[0]));
	//if (iBtRGB[0] == iRGB[0])
		return true;
	//else
		//return false;
}
#define VERTEXUICOLOR_THEME RGB(228,212,255)

void VuiColorSystemInit()
{
	vuicolor.bg_d1_r = SetColorBright(vuicolor.bg_r, 10);
	vuicolor.bg_d1_g = SetColorBright(vuicolor.bg_g, 10);
	vuicolor.bg_d1_b = SetColorBright(vuicolor.bg_b, 10);
	vuicolor.bg_d2_r = SetColorBright(vuicolor.bg_r, 30);
	vuicolor.bg_d2_g = SetColorBright(vuicolor.bg_g, 30);
	vuicolor.bg_d2_b = SetColorBright(vuicolor.bg_b, 30);
	unsigned long newColor = VuiAdaptColor(RGB(vuicolor.bg_d1_r, vuicolor.bg_d1_g, vuicolor.bg_d1_b));
	vuicolor.bg_d1_r = GetRValue(newColor);
	vuicolor.bg_d1_g = GetGValue(newColor);
	vuicolor.bg_d1_b = GetBValue(newColor);
	if (vuicolor.bg_r <= 128)
	{
		vuicolor.title.bg_r = vuicolor.bg_r;
		vuicolor.title.bg_g = vuicolor.bg_g;
		vuicolor.title.bg_b = vuicolor.bg_b;
	}
	else
	{
		vuicolor.title.bg_r = vuicolor.title.oR;
		vuicolor.title.bg_g = vuicolor.title.oG;
		vuicolor.title.bg_b = vuicolor.title.oB;
	}
	vuicolor.title.bg_d1_r = SetColorBright(vuicolor.title.bg_r, 10);
	vuicolor.title.bg_d1_g = SetColorBright(vuicolor.title.bg_g, 10);
	vuicolor.title.bg_d1_b = SetColorBright(vuicolor.title.bg_b, 10);
	vuicolor.title.bg_d2_r = SetColorBright(vuicolor.title.bg_r, 30);
	vuicolor.title.bg_d2_g = SetColorBright(vuicolor.title.bg_g, 30);
	vuicolor.title.bg_d2_b = SetColorBright(vuicolor.title.bg_b, 30);


	vuicolor.ctl_d1_r = SetColorBright(vuicolor.bg_r, 35);
	vuicolor.ctl_d1_g = SetColorBright(vuicolor.bg_g, 35);
	vuicolor.ctl_d1_b = SetColorBright(vuicolor.bg_b, 35);
	vuicolor.ctl_blur_r = SetColorBright(vuicolor.bg_r,13);
	vuicolor.ctl_blur_g = SetColorBright(vuicolor.bg_g,13);
	vuicolor.ctl_blur_b = SetColorBright(vuicolor.bg_b,13);
	
	if (VuiColorAverage(RGB(vuicolor.bg_r, vuicolor.bg_g, vuicolor.bg_b)) < 128)
	{
		vuicolor.txt_r = 255;
		vuicolor.txt_g = 255;
		vuicolor.txt_b = 255;
	}
	else
	{
		vuicolor.txt_r = 20;
		vuicolor.txt_g = 20;
		vuicolor.txt_b = 20;
	}
	
}

#define VERTEXUICOLOR_DARKEN RGB(20,20,20)

#define VERTEXUICOLOR_TABDARKEN RGB(57, 62, 71)

#define VERTEXUICOLOR_WHITEDRAW RGB(244,244,244)

#define VERTEXUICOLOR_WHITE RGB(vuicolor.txt_r,vuicolor.txt_g,vuicolor.txt_b)

#define VERTEXUICOLOR_SEA /*RGB(45,137,239)*/RGB(82, 121, 251)

#define VERTEXUICOLOR_DEEPSEA RGB(15,107,209)

#define VERTEXUICOLOR_LAVENDER RGB(108,92,200)

#define VERTEXUICOLOR_LAVENDER_ RGB(128,112,220)

#define VERTEXUICOLOR_BLOOMLAVENDER RGB(158,142,250)

#define VERTEXUICOLOR_GREENSEA RGB(26,188,156)

#define VERTEXUICOLOR_GREENDEEPSEA RGB(4,138,106)

#define VERTEXUICOLOR_REALLYGREENDEEPSEA RGB(68,102,103)

#define VERTEXUICOLOR_DARKENX RGB(10,10,10)

#define VERTEXUICOLOR_WARNING RGB(222, 119, 64)

#define VERTEXUICOLOR_PEACHRED RGB(232,77,61)

#define VERTEXUICOLOR_MIDNIGHT RGB(vuicolor.bg_d1_r, vuicolor.bg_d1_g, vuicolor.bg_d1_b)

#define VERTEXUICOLOR_MIDNIGHTPLUS RGB(vuicolor.bg_d2_r, vuicolor.bg_d2_g, vuicolor.bg_d2_b)

#define VERTEXUICOLOR_DARKNIGHT RGB(vuicolor.bg_r, vuicolor.bg_g, vuicolor.bg_b)

#define VERTEXUICOLOR_FOREST RGB(38,212,110)

#define VERTEXUICOLOR_DAWN RGB(255,115,119)

#define VERTEXUICOLOR_LDARK RGB(42+35,47+35,56+35)

#define VERTEXUICOLOR_OTHER_GITHUB RGB(88,166,255)

#define VERTEXUI_DARKEN RGB(20,20,20)

#define VERTEXUI_WHITE RGB(244,244,244)

#define VERTEXUI_GREENSEA RGB(26,188,156)

#define VERTEXUICOLOR_SHADOWNIGHT RGB(57,62,71)


COLORREF VertexUIColor_Main;