#pragma once

struct vec3 {
	float x, y, z;

	vec3 operator-(const vec3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
};

struct vec2 {
	float x, y;
};

struct viewMatrix {
	float matrix[16];
};

struct clip {
	float x, y, z, w;
};
