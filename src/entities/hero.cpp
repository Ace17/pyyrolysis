// Copyright (C) 2018 - Sebastien Alaiwan
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

#include <algorithm>

#include "base/scene.h"
#include "base/util.h"

#include "collision_groups.h"
#include "entities/player.h"
#include "entities/move.h"
#include "entity.h"
#include "models.h"
#include "sounds.h"
#include "toggle.h"
#include "hero.h"
#include "trigger.h"

auto const WALK_SPEED = 0.02f;
auto const MAX_SPEED = 0.02f;
auto const HURT_DELAY = 500;
auto const MAX_LIFE = 2000;

static auto const NORMAL_SIZE = UnitSize * 1.5;
static auto const DEFAULT_ORIENTATION = Orientation { Vector3f(1, 0, 0), Vector3f(0, 0, 1) };

struct Hero : Player, Damageable
{
  Hero()
  {
    size = NORMAL_SIZE;
    orientation = DEFAULT_ORIENTATION;
  }

  void enter() override
  {
    respawnPoint = pos;
  }

  virtual Actor getActor() const override
  {
    auto r = Actor(pos, MDL_INVRECT);
    r.scale = size;
    r.focus = true;
    auto health = clamp(life / float(MAX_LIFE), 0.0f, 1.0f);

    if(health > 0)
    {
      auto fuzzFactor = 1.0 - health;
      auto fuzzRange = 1.5;

      auto randomFloat = [] (float min, float max)
        {
          float range = max - min;
          return (rand() / float(RAND_MAX)) * range + min;
        };

      r.pos.x += randomFloat(-fuzzRange, fuzzRange) * fuzzFactor;
      r.pos.y += randomFloat(-fuzzRange, fuzzRange) * fuzzFactor;
      r.pos.z += randomFloat(-fuzzRange, fuzzRange) * fuzzFactor;
    }

    if(1) // hide debug box
    {
      r.action = 0;
    }
    else
    {
      r.action = 1;
    }

    r.orientation = orientation;

    return r;
  }

  void think(Control const& c) override
  {
    control = c;
  }

  float health() override
  {
    return clamp(life / float(MAX_LIFE), 0.0f, 1.0f);
  }

  virtual void addUpgrade(int upgrade) override
  {
    upgrades |= upgrade;
  }

  void computeVelocity(Control c)
  {
    airMove(c);

    vel.x = clamp(vel.x, -MAX_SPEED, MAX_SPEED);
    vel.y = clamp(vel.y, -MAX_SPEED, MAX_SPEED);
    vel.z = clamp(vel.z, -MAX_SPEED, MAX_SPEED);
  }

  void airMove(Control c)
  {
    auto const forward = orientation.dir;
    auto const left = crossProduct(orientation.up, orientation.dir);

    Vector wantedVel = Vector(0, 0, 0);

    if(c.backward)
      wantedVel -= forward * WALK_SPEED;

    if(c.forward)
      wantedVel += forward * WALK_SPEED;

    if(c.left)
      wantedVel += left * WALK_SPEED;

    if(c.right)
      wantedVel -= left * WALK_SPEED;

    vel = vel * 0.997 + wantedVel * 0.003;

    if(abs(vel.x) < 0.00001)
      vel.x = 0;

    if(abs(vel.y) < 0.00001)
      vel.y = 0;

    if(abs(vel.z) < 0.00001)
      vel.z = 0;
  }

  virtual void tick() override
  {
    if(life <= 0)
    {
      control = Control {};
      control.look_vert = +0.003;
      control.look_horz = -0.002;
    }
    else
      life = min(MAX_LIFE, life + 1);

    auto const speed = 0.2;
    auto const left = crossProduct(orientation.up, orientation.dir);
    auto const fwd = crossProduct(left, orientation.up);
    orientation.dir += (orientation.up * control.look_vert + left * control.look_horz) * speed;
    orientation.up += (fwd * control.look_vert) * speed;

    auto removeNormalComponent = [] (Vector3f v, Vector3f normal)
      {
        return v - normal * dotProduct(normal, v);
      };

    // keep 'dir' and 'up' orthonormal
    orientation.dir = normalize(orientation.dir);
    orientation.up = normalize(removeNormalComponent(orientation.up, orientation.dir));

    if(decrement(deathDelay))
    {
      auto evt = make_unique<PlayerDiedEvent>();
      game->postEvent(move(evt));
    }

    if(decrement(breatheDelay) || breatheDelay == 0)
    {
      game->playSound(SND_BREATHE);
      breatheDelay = 6000;
    }

    computeVelocity(control);

    slideMove(physics, this, vel);

    auto const onGround = isOnGround(physics, this);

    if(!onGround)
    {
      ground = false;
    }
    else
    {
      if(vel.z < 0 && !ground)
      {
        if(tryActivate(debounceLanding, 150))
          game->playSound(SND_LAND);

        ground = true;
        vel.z = 0;
      }
    }

    decrement(debounceLanding);
    decrement(debounceDamage);
    decrement(debounceUse);

    if(debounceDamage)
      game->addAmbientLight(100);

    if(control.use && debounceUse == 0)
    {
      debounceUse = 200;

      // look in front of us for a body to switch,
      // and switch it.
      auto const forward = orientation.dir;
      Box box = getBox();
      auto body = physics->traceBox(box, forward, this).blocker;

      if(auto switchable = dynamic_cast<Switchable*>(body))
        switchable->onSwitch();
    }

    collisionGroup = CG_PLAYER;
    collisionGroup |= CG_SOLIDPLAYER;
  }

  virtual void onDamage(int amount) override
  {
    if(life <= 0)
      return;

    game->textBox("KEEP AWAY!");
    life -= amount;

    if(debounceDamage == 0)
    {
      debounceDamage = 100;
      game->playSound(SND_DAMAGE);
    }

    if(life <= 0)
    {
      die();
      return;
    }
  }

  void die()
  {
    game->playSound(SND_DIE);
    deathDelay = 5000;
  }

  int breatheDelay = 0;
  int debounceLanding = 0;
  int debounceUse = 0;
  int debounceDamage = 0;
  Orientation orientation;
  bool ground = false;
  Toggle jumpbutton, firebutton;
  int life = MAX_LIFE;
  Control control {};

  int deathDelay = 0;
  Vector respawnPoint;
  Vector vel;

  int upgrades = 0;
};

std::unique_ptr<Player> makeHero()
{
  return make_unique<Hero>();
}

