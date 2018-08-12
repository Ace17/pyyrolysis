#pragma once

#include "base/util.h"
#include "base/scene.h"
#include "collision_groups.h"
#include "entity.h"
#include "models.h"

struct Heater : Entity
{
  Heater()
  {
    solid = 1;
    size = UnitSize;
    collisionGroup = CG_WALLS;
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos, MDL_HEATER);
    r.scale = size;
    if(0)
      r.effect = Effect::Blinking;

    return r;
  }

  void enter() override
  {
  }

  void tick() override
  {
  }

  int disappearTimer = 0;
};

