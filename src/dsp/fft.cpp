#include "fft.h"

#define M_PI 3.1415926535897932384626

void bit_reverse(complex_f32_t* a, int n)
{
	for (int i = 1, j = 0; i < n - 1; ++i)
	{
		for (int s = n; j ^= s >>= 1, ~j & s;)
			;
		if (i < j)
		{
			complex_f32_t tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}
}
void fft_f32(complex_f32_t* a, int n, int inv)
{
	bit_reverse(a, n);
	complex_f32_t t;
	int p1, p2;
	for (int m = 2; m <= n; m <<= 1)
	{
		double theta = 2.0f * M_PI / l;
		complex_f32_t omega = cpolar(1.0f, theta);

		for (int i = 0; i < n; i += m)
		{
			complex_f32_t w = {1.0f, 0.0f};
			p1 = m >> 1;

			for (int j = 0; j < p1; ++j)
			{
				p2 = i + j;
				// 计算 t = w * a[p2 + p1]
				complex_f32_t t = cmul(w, a[p2 + p1]);

				// 更新 a[p2] 和 a[p2 + p1]
				a[p2 + p1] = csub(a[p2], t);
				a[p2] = cadd(a[p2], t);

				// 更新 w = w * wm
				t = cmul(w, omega);
				w = t;
			}
		}
	}
}