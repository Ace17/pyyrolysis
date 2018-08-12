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
  }

  void onSwitch() override
  {
    if(state)
      return;

    state = true;
    game->playSound(SND_SWITCH);

    auto evt = make_unique<TriggerEvent>();
    evt->idx = id;
    game->postEvent(move(evt));
  }

  bool state = false;
  const int id;
};

