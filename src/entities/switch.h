// Copyright (C) 2018 - Sebastien Alaiwan
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// switch and doors entities.

#pragma once

#include "entity.h"

unique_ptr<Entity> makeSwitch(int id);
unique_ptr<Entity> makeDoor(int id);
unique_ptr<Entity> makeAutoDoor();
unique_ptr<Entity> makeBreakableDoor();

