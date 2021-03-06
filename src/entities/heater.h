#pragma once

#include "base/util.h"
#include "base/scene.h"
#include "collision_groups.h"
#include "entity.h"
#include "trigger.h"
#include "models.h"
#include "heatbox.h"

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
    game->playSound(SND_ENGINE);

    auto evt = make_unique<TriggerEvent>();
    evt->idx = id;
    game->postEvent(move(evt));

    auto heatBox = make_unique<HeatBox>();
    heatBox->pos = getCenter();
    game->spawn(heatBox.release());
  }

  bool state = false;
  const int id;
};

