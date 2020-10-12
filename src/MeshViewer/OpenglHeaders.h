#ifndef OPENGL_HEADERS_H
#define OPENGL_HEADERS_H

#include <QGLWidget>

#ifdef _WIN64
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif __APPLE__
#include <GLUT/glut.h>
#endif
#endif

