#pragma once

#include "structs.h"

bool worldToScreen(vec3 pos, vec2& screen, viewMatrix matrix, int windowWidth, int windowHeight);
float getDistance3D(vec3 from, vec3 to);