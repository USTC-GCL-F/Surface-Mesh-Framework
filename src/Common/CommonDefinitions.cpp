#include "CommonDefinitions.h"

const int CommonDefinition::colorDisplay[5][3] = { {229,16,59}, {33,75,203}, {33,203,75},{127,0,255},{255,70,0} };

const char CommonDefinition::Color_String[5][512] = {"color: rgb(229,16,59)", 
	"color: rgb(33,75,203)", "color: rgb(33,203,75)", "color: rgb(127,0,255)", "color: rgb(255,70,0)"};

const int CommonDefinition::PointColor[2][3] = { {128,0,42}, {128,170,0} };

const char CommonDefinition::PointColor_String[2][512] = {"color: rgb(128,0,42)", "color: rgb(128,170,0)"};

const int CommonDefinition::SingularityColor[12][3] = {{255,85,0}, {255,170,0}, {255,255,0},{170,255,0},
													   {0,255,255},{0,255,170},{0,85,255},{0,170,255},
													   {85,0,255},{170,0,255},{255,0,85},{255,0,170}};
