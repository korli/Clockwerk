#ifndef UI_DEFINES_H
#define UI_DEFINES_H

#include <GraphicsDefs.h>

const rgb_color kBlack			= {   0,   0,   0, 255 };
const rgb_color kWhite			= { 255, 255, 255, 255 };

const rgb_color kRed			= { 255,   0,   0, 255 };
const rgb_color kGreen			= {   0, 255,   0, 255 };
const rgb_color kBlue			= {   0,   0, 255, 255 };

const rgb_color kOrange			= { 255, 217, 121, 255 };
const rgb_color kLightOrange	= { 255, 217, 138, 255 };
const rgb_color kDarkOrange		= { 255, 145,  71, 255 };

const pattern kStripes			= { { 0xc7, 0x8f, 0x1f, 0x3e, 0x7c, 0xf8, 0xf1, 0xe3 } };
const pattern kDotted			= { { 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa } };
const pattern kDottedBigger		= { { 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc } };
const pattern kDottedBig		= { { 0x0f, 0x0f, 0x0f, 0x0f, 0xf0, 0xf0, 0xf0, 0xf0 } };

enum {
	MSG_FOCUS_CHANGED			= '_fcc',
};

#endif // UI_DEFINES_H
