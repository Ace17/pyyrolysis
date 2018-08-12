#pragma once

#include "base/util.h"
#include "base/scene.h"
#include "collision_groups.h"
#include "entity.h"
#include "trigger.h"
#include "models.h"

struct CentralHeater : Entity, IEventSink
{
  CentralHeater()
  {
    size = UnitSize * 4;
    solid = true;
  }

  void enter() override
  {
    subscription = game->subscribeForEvents(this);
  }

  void leave() override
  {
    subscription.reset();
  }

  virtual void tick() override
  {
    if(remaining == 0)
    {
      counter += 0.001;
      game->addAmbientLight(exp(counter));
    }

    if(counter > 4.0)
      game->endLevel();
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos, MDL_HEATER);
    r.scale = size;

    if(remaining == 0)
      r.effect = Effect::Blinking;

    return r;
  }

  virtual void notify(const Event* evt) override
  {
    if(evt->as<TriggerEvent>())
    {
      --remaining;

      if(remaining == 0)
      {
        game->textBox("pyyrolysis sequence initiated");
      }
      else
      {
        char buffer[256];
        sprintf(buffer, "%d remaining ...", remaining);
        game->textBox(buffer);
      }
    }
  }

  float counter = 0;
  int remaining = 17;
  unique_ptr<Handle> subscription;
};

