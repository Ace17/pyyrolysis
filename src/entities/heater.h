#pragma once

#include "base/util.h"
#include "base/scene.h"
#include "collision_groups.h"
#include "entity.h"
#include "trigger.h"
#include "models.h"

struct Heater : Entity, Switchable
{
  Heater(int id_) : id(id_)
  {
    solid = 1;
    size = UnitSize;
    collisionGroup = CG_WALLS;
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos, MDL_HEATER);
    r.scale = size;

    if(state)
      r.effect = Effect::Blinking;

    return r;
  }

  void enter() override
  {
  }

  void tick() override
  {
    if(state)
    {
      m_emitPower += 0.01;
      auto player = game->getPlayerPosition();
      auto delta = player - pos;
      auto invDist = m_emitPower / dotProduct(delta, delta);

      if(invDist > 2 && 0)
        game->textBox("KEEP AWAY!");

      game->addAmbientLight(invDist);
    }
  }

  void onSwitch() override
  {
    if(state)
      return;

    state = true;
    game->playSound(SND_SWITCH);
    game->playSound(SND_ENGINE);

    auto evt = make_unique<TriggerEvent>();
    evt->idx = id;
    game->postEvent(move(evt));
  }

  float m_emitPower = 0;
  bool state = false;
  const int id;
};

