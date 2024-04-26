#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <flecs.h>

#include <cglm/ivec2.h>
#include <cglm/ivec3.h>
#include <cglm/ivec4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>

#include "types.h"

#define IDIOMS_VERSION_MAJOR 0
#define IDIOMS_VERSION_MINOR 5
#define IDIOMS_VERSION_PATCH 0

#define LOG_TRACE(...) ecs_log_(00, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  ecs_log_(-2, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) ecs_log_(-3, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) ecs_log_(-4, __FILE__, __LINE__, __VA_ARGS__)