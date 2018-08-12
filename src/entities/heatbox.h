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
    size.cx += 0.0005;
    size.cy += 0.0005;
    size.cz += 0.0005;
    pos = center - size * 0.5;

    m_emitPower += 0.01;
    auto player = game->getPlayerPosition();
    auto delta = player - pos;
    auto invDist = m_emitPower / dotProduct(delta, delta);

    game->addAmbientLight(invDist);
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

