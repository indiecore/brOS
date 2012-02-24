#include "math.h"

double sqrt(double x) {
	double ret;
	asm volatile("fldl %[input]; fsqrt; fstpl %[output]" : [output] "=g" (ret) : [input] "g" (x));
	return ret;
}

double sin(double theta) {
	double x;
	asm volatile("fldl %[input]; fsin; fstpl %[output];" : [output] "=g" (x)  : [input] "g" (theta));
	return x;
}

double cos(double theta) {
	double x;
	asm volatile("fldl %[input]; fcos; fstpl %[output];" : [output] "=g" (x)  : [input] "g" (theta));
	return x;
}

double tan(double theta) {
	double x;
	asm volatile("fldl %[input]; fptan; fstpl %[output]; fstpl %[output]" : [output] "=g" (x) : [input] "g" (theta));
	return x;
}

double atan(double x) {
	double ret;
	asm volatile("fldl %[input]; fld1; fpatan; fstpl %[output]" : [output] "=g" (ret) : [input] "g" (x));
	return ret;
}

double asin(double x) {
	return 2.0*atan(x/(1.0+sqrt(1.0-(x*x))));
}

double acos(double x) {
	return 2.0*atan(sqrt(1.0 - x*x)/(1.0+x));
}

double atan2(double x, double y) {
	double ret;
	asm volatile("fldl %[y]; fldl %[x]; fpatan; fstpl %[output]" : [output] "=g" (ret) : [x] "g" (x), [y] "g" (y));
	return ret;
}



//log base 2
double lg(double x) {
	double ret;
	asm volatile("fld1; fldl %[input] ; fyl2x; fstpl %[output]" : [output] "=g" (ret) : [input] "g" (x));
	return ret;
}

double abs(double x)
{
	if(x<0.0)
		return x*-1.0;
	else
		return x;
}
//natural log
double ln(double x) {
	double ret;
	return ret;
}

double log(double x, double base) {
	double ret;
	asm volatile("fld1; fldl %[base] ; fyl2x; fld1; fldl %[input]; fyl2x; fdivp; fstpl %[output]" : [output] "=g" (ret) : [input] "g" (x), [base] "g" (base));
	return ret;
}

double log10(double x) {
	double ret;
	double base = 10.0;
	asm volatile("fld1; fldl %[base] ; fyl2x; fld1; fldl %[input]; fyl2x; fdivp; fstpl %[output]" : [output] "=g" (ret) : [input] "g" (x), [base] "g" (base));
	return ret;
}
