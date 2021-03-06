// Copyright (C) 2018 - Sebastien Alaiwan <sebastien.alaiwan@gmail.com>
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

#pragma once

#include <stdint.h>
#include <vector>
#include <string>
using namespace std;

#include "base/geom.h"

struct Action
{
  vector<int> textures;
  void addTexture(string path, Rect2i rect = Rect2i(0, 0, 0, 0));
};

struct Model
{
  uint32_t buffer = 0;
  vector<Action> actions;

  // mesh data
  struct Vertex
  {
    float x, y, z, u, v, nx, ny, nz;
  };

  vector<Vertex> vertices;
};

Model loadModel(string jsonPath);

