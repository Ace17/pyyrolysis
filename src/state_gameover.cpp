// Copyright (C) 2018 - Sebastien Alaiwan <sebastien.alaiwan@gmail.com>
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// splash menu

#include "base/scene.h"
#include "base/view.h"

#include "toggle.h"
#include "sounds.h" // SND_TELEPORT
#include "state_machine.h"

struct GameOverState : Scene
{
  GameOverState(View* view_, StateMachine* fsm_, char const* msg_) : view(view_), fsm(fsm_), msg(msg_)
  {
  }

  ////////////////////////////////////////////////////////////////
  // Scene: Game, seen by the engine

  void tick(Control const& c) override
  {
    auto const FADE_TIME = 2000;

    view->playMusic(1);

    if(!activated)
    {
      view->textBox(msg);

      if(c.fire || c.jump || c.dash)
      {
        activated = true;
        delay = FADE_TIME;
        view->playSound(SND_TELEPORT);
      }
    }

    if(activated)
    {
      ambientLight = delay / float(FADE_TIME);

      if(decrement(delay))
      {
        activated = false;
        fsm->next();
      }
    }
    else
    {
      ambientLight = 10;
    }
  }

  vector<Actor> getActors() const override
  {
    return vector<Actor>();
  }

private:
  View* const view;
  StateMachine* const fsm;
  const char* const msg;
  bool activated = false;
  int delay = 0;
};

unique_ptr<Scene> createGameOverState(StateMachine* fsm, View* view, char const* msg)
{
  return make_unique<GameOverState>(view, fsm, msg);
}

