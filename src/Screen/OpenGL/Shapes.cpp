/*
Copyright_License {

  Top Hat Soaring Glide Computer - http://www.tophatsoaring.org/
  Copyright (C) 2000-2016 The Top Hat Soaring Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Screen/OpenGL/Shapes.hpp"
#include "Screen/OpenGL/FallbackBuffer.hpp"
#include "Math/FastTrig.hpp"
#include "Math/Point2D.hpp"

#include <assert.h>

namespace OpenGL {
  GLFallbackArrayBuffer *circle_buffer, *small_circle_buffer;
}

static GLFallbackArrayBuffer *
MakeCircleBuffer(unsigned n)
{
  assert(4096 % n == 0);

  auto buffer = new GLFallbackArrayBuffer();

  FloatPoint *const p0 = (FloatPoint *)buffer->BeginWrite(sizeof(*p0) * n);
  FloatPoint *p = p0, *p2 = p + n / 2;

  for (unsigned i = 0; i < n / 2; ++i, ++p, ++p2) {
    float x = ISINETABLE[(i * (4096 / n) + 1024) & 0xfff] / 1024.;
    float y = ISINETABLE[i * (4096 / n)] / 1024.;

    p->x = x;
    p->y = y;

    p2->x = -x;
    p2->y = -y;
  }

  buffer->CommitWrite(sizeof(*p0) * n, p0);
  return buffer;
}

void
OpenGL::InitShapes()
{
  DeinitShapes();

  assert(4096 % CIRCLE_SIZE == 0);  // implies: assert(SIZE % 2 == 0)

  circle_buffer = MakeCircleBuffer(CIRCLE_SIZE);
  small_circle_buffer = MakeCircleBuffer(SMALL_CIRCLE_SIZE);
}

void
OpenGL::DeinitShapes()
{
  delete circle_buffer;
  circle_buffer = nullptr;

  delete small_circle_buffer;
  small_circle_buffer = nullptr;
}
