/* Maze 3D - A simplistic random maze generator.
 * Copyright (C) 2016 Armend Murtishi

 * This file is part of Maze 3D.

 * Maze 3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Maze 3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Maze 3D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHADER1_H

#define SHADER1_H

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

class Shader1
{
public:
    GLuint program;

    void use();

    Shader1(const GLchar* vert, const GLchar* frag);
    ~Shader1();
};

#endif /* end of include guard: SHADER1_H */
