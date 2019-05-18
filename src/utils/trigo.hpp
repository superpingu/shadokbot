/* trigo.h/trigo.c define table based trigonometry functions
 * allowing to save precious flash space while keeping a good precision */

#ifndef TRIGO_H
#define TRIGO_H

float msin(float x);
float mcos(float x);
float msin_deg(int x);
float mcos_deg(int x);
float macos(float x);
float masin(float x);
float matan(float x);

#endif
