#include "complex.h"
#include <math.h>

complex_f32_t cadd(complex_f32_t a, complex_f32_t b) { return complex_f32_t{ a.re + b.re, a.im + b.im }; }
complex_f32_t csub(complex_f32_t a, complex_f32_t b) { return complex_f32_t{ a.re - b.re, a.im - b.im }; }
complex_f32_t cmul(complex_f32_t a, complex_f32_t b) { return complex_f32_t{ a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re }; }
complex_f32_t cmulreal(complex_f32_t a, float b) { return complex_f32_t{ a.re * b, a.im * b }; }

complex_f32_t cdiv(complex_f32_t a, complex_f32_t b)
{
	float x = b.re * b.re + b.im * b.im;
	return complex_f32_t{ (a.re * b.re + a.im * b.im) / x,(a.im * b.re - a.re * b.im) / x };
}
complex_f32_t cpolar(float sz, float ang) { return complex_f32_t{ sz * cosf(ang), sz * sinf(ang) }; }
float csize(complex_f32_t a) { return sqrtf(a.re * a.re + a.im * a.im); }
float cangle(complex_f32_t a) { return atanf(a.re / a.im); }

complex_f64_t cadd64(complex_f64_t a, complex_f64_t b) { return complex_f64_t{ a.re + b.re, a.im + b.im }; }
complex_f64_t csub64(complex_f64_t a, complex_f64_t b) { return complex_f64_t{ a.re - b.re, a.im - b.im }; }
complex_f64_t cmul64(complex_f64_t a, complex_f64_t b) { return complex_f64_t{ a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re }; }
complex_f64_t cmulreal64(complex_f64_t a, double b) { return complex_f64_t{ a.re * b, a.im * b }; }

complex_f64_t cdiv64(complex_f64_t a, complex_f64_t b)
{
	double x = b.re * b.re + b.im * b.im;
	return complex_f64_t{ (a.re * b.re + a.im * b.im) / x,(a.im * b.re - a.re * b.im) / x };
}
complex_f64_t cpolar64(double sz, double ang) { return complex_f64_t{ sz * cos(ang), sz * sin(ang) }; }
double csize64(complex_f64_t a) { return sqrt(a.re * a.re + a.im * a.im); }
double cangle64(complex_f64_t a) { return atan(a.re / a.im); }