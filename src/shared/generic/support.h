/*
 * Copyright 2001-2008, Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef SUPPORT_H
#define SUPPORT_H

#include <Rect.h>
#include <String.h>

class BPositionIO;
class BString;

// constrain
inline void
constrain(float& value, float min, float max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
}

// constrain_int32_0_255_asm
inline int32
constrain_int32_0_255_asm(int32 value) {
	asm("movl  $0,    %%ecx;"
		"movl  $255,  %%edx;"
		"cmpl  %%ecx, %%eax;"
		"cmovl %%ecx, %%eax;"
		"cmpl  %%edx, %%eax;"
		"cmovg %%edx, %%eax"
		: "=a" (value)
		: "a" (value) 
		: "%ecx", "%edx");
    return value;
}

inline int32
constrain_int32_0_255_c(int32 value) {
    return max_c(0, min_c(255, value));
}

#define constrain_int32_0_255 constrain_int32_0_255_asm

// rect_to_int
inline void
rect_to_int(BRect r,
			int32& left, int32& top, int32& right, int32& bottom)
{
	left = (int32)floorf(r.left);
	top = (int32)floorf(r.top);
	right = (int32)ceilf(r.right);
	bottom = (int32)ceilf(r.bottom);
}

// point_point_distance
inline float
point_point_distance(BPoint a, BPoint b)
{
	float xDiff = b.x - a.x;
	float yDiff = b.y - a.y;
	return sqrtf(xDiff * xDiff + yDiff * yDiff);
}

// point_line_distance
double
point_line_distance(double x1, double y1,
					double x2, double y2,
					double x,  double y);

// point_line_distance
double
point_line_distance(BPoint point, BPoint a, BPoint b);

// calc_angle
double
calc_angle(BPoint origin, BPoint from, BPoint to, bool degree = true);

/*
template <class T>
T min4(const T a, const T b, const T c, const T d)
{
	T e = a < b ? a : b;
	T f = c < d ? c : d;
	return e < f ? e : f;
} 
template <class T>
T max4(const T a, const T b, const T c, const T d)
{
	T e = a > b ? a : b;
	T f = c > d ? c : d;
	return e > f ? e : f;
} 
*/
inline float
min4(float a, float b, float c, float d)
{
	return min_c(a, min_c(b, min_c(c, d)));
} 

inline float
max4(float a, float b, float c, float d)
{
	return max_c(a, max_c(b, max_c(c, d)));
} 

inline float
min5(float v1, float v2, float v3, float v4, float v5)
{
	return min_c(min4(v1, v2, v3, v4), v5);
}

inline float
max5(float v1, float v2, float v3, float v4, float v5)
{
	return max_c(max4(v1, v2, v3, v4), v5);
}

inline float
roundf(float v)
{
	if (v >= 0.0f)
		return floorf(v + 0.5f);
	return ceilf(v - 0.5f);
}

status_t write_string(BPositionIO* stream, BString& string);
void append_float(BString& string, float n, int32 maxDigits = 4);

double gauss(double f);

void string_for_time_of_day(BString& string, uint32 secondOfDay);
void string_for_frame_of_day(BString& string, uint64 frameOfDay);
const char* print_week(uint8 days, BString& string);

bool string_ends_with(const BString& string, const char* match);

# endif // SUPPORT_H
