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
    solid = 0;
    size = UnitSize * 0.5;
    collisionGroup = CG_ALL;
    collidesWith = CG_PLAYER;
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos, MDL_RECT);
    r.scale = size;

    return r;
  }

  void enter() override
  {
    Body::onCollision = [this] (Body* other) { touch(other); };
    center = pos;
  }

  void tick() override
  {
    size.cx += 0.001;
    size.cy += 0.001;
    size.cz += 0.001;
    pos = center - size * 0.5;
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
  Vector center;
};

