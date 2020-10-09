#ifndef COMMON_DEFINITION_H
#define COMMON_DEFINITION_H

#include <limits>

namespace CommonDefinition
{
	enum COLOR_Definition{RED=0,BLUE,GREEN,PURPLE,ORANGE,N_COLOR};
	extern const int colorDisplay[5][3];
	extern const char Color_String[5][512];
	extern const int PointColor[2][3];
	extern const char PointColor_String[2][512];
	extern const int SingularityColor[12][3];
}

#endif