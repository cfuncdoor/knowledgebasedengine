#pragma once

#include "core/types/types.h"
#include "core/ecs/world.h"
#include "core/renderer/renderer.h"
#include "core/math/matrix/matrix.h"

void render_system_update(renderer_t* renderer, world* w);