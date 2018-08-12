#pragma once

#include "base/util.h"
#include "base/scene.h"
#include "collision_groups.h"
#include "entity.h"
#include "trigger.h"
#include "models.h"

struct HeatBox : Entity
{
  HeatBox()
  {
    solid = 1;
    size = UnitSize * 3;
    collisionGroup = CG_ALL;
    collidesWith = CG_PLAYER;
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos - size * 0.5, MDL_HEATER);
    r.scale = size;

    return r;
  }

  void enter() override
  {
    Body::onCollision = [this] (Body* other) { touch(other); };
  }

  void tick() override
  {
  }

  void touch(Body* other)
  {
    if(auto damageable = dynamic_cast<Damageable*>(other))
    {
      damageable->onDamage(2);
    }
  }

  float m_emitPower = 0;
  bool state = false;
};

