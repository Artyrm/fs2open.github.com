#pragma once

#include "globalincs/pstypes.h"

namespace graphics {

/**
 * @file
 *
 * This file contains definitions for GPU uniform buffer structs. These structs must respect the std140 layout rules.
 * Read the OpenGL specification for the exact layout and padding rules.
 */

/**
 * @brief Data for one deferred light rendering call
 */
struct deferred_light_data {
	vec3d diffuseLightColor;
	float coneAngle;

	vec3d specLightColor;
	float coneInnerAngle;

	vec3d coneDir;
	float dualCone;

	vec3d scale;
	float lightRadius;

	int lightType;

	int pad0[3]; // Struct size must be 16-bytes aligned because we use vec3s
};

struct decal_globals {
	matrix4 viewMatrix;
	matrix4 projMatrix;
	matrix4 invViewMatrix;
	matrix4 invProjMatrix;

	vec2d viewportSize;

	float pad[2];
};

struct decal_info {
	matrix4 model_matrix;
	matrix4 inv_model_matrix;

	vec3d decal_direction;
	float normal_angle_cutoff;

	int diffuse_index;
	int normal_index;
	float angle_fade_start;
	float alpha_scale;
};

}
