#pragma once

#include "game.h"

struct TriggerEvent : Event
{
  int idx {};
};

struct PlayerDiedEvent : Event
{
};

