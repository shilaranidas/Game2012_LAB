﻿#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "glm\glm.hpp"
#include "Light1.h"
#define PI 3.14159265358979324
using namespace std;

struct Shape
{
	vector<GLshort> shape_indices;
	vector<GLfloat> shape_vertices;
	vector<GLfloat> shape_colors;
	vector<GLfloat> shape_uvs;
	vector<GLfloat> shape_normals;
	Material shape_mat = { 1.0f, 32 };

	~Shape()
	{
		shape_indices.clear();
		shape_indices.shrink_to_fit();
		shape_vertices.clear();
		shape_vertices.shrink_to_fit();
		shape_colors.clear();
		shape_colors.shrink_to_fit();
		shape_uvs.clear();
		shape_uvs.shrink_to_fit();
		shape_normals.clear();
		shape_normals.shrink_to_fit();
	}
	GLsizei NumIndices() { return shape_indices.size(); }
	void BufferShape(GLuint* ibo, GLuint* points_vbo, GLuint* colors_vbo, GLuint* uv_vbo, GLuint* normals_vbo, GLuint& program)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape_indices[0]) * shape_indices.size(), &shape_indices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, *points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shape_vertices[0]) * shape_vertices.size(), &shape_vertices.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(shape_vertices[0]) * 3, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, *colors_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shape_colors[0]) * shape_colors.size(), &shape_colors.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, *uv_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shape_uvs[0]) * shape_uvs.size(), &shape_uvs.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, *normals_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shape_normals[0]) * shape_normals.size(), &shape_normals.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), shape_mat.specularStrength);
		glUniform1f(glGetUniformLocation(program, "mat.shininess"), shape_mat.shininess);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void ColorShape(GLfloat r, GLfloat g, GLfloat b)
	{
		shape_colors.clear();
		shape_colors.shrink_to_fit();
		for (int i = 0; i < shape_vertices.size(); i += 3)
		{
			shape_colors.push_back(r);
			shape_colors.push_back(g);
			shape_colors.push_back(b);
		}
		shape_colors.shrink_to_fit(); // Good idea after a bunch of pushes.
	}
	void CalcAverageNormals(vector<GLshort>& indices, unsigned indiceCount, vector<GLfloat>& vertices,
		unsigned verticeCount)
	{
		// Popular shape_normals so we can use [].
		for (int i = 0; i < verticeCount; i++)
			shape_normals.push_back(0);
		shape_normals.shrink_to_fit();
		// Calculate the normals of each triangle first.
		for (unsigned i = 0; i < indiceCount; i += 3)
		{
			unsigned int in0 = indices[i] * 3;
			unsigned int in1 = indices[i + 1] * 3;
			unsigned int in2 = indices[i + 2] * 3;
			glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
			glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
			glm::vec3 normal = glm::cross(v2, v1);
			normal = glm::normalize(normal); // Finally becomes a unit vector.

			// Now populate the normal values for each vertex of the triangle.
			shape_normals[in0] += normal.x;	shape_normals[in0 + 1] += normal.y;	shape_normals[in0 + 2] += normal.z;
			shape_normals[in1] += normal.x;	shape_normals[in1 + 1] += normal.y;	shape_normals[in1 + 2] += normal.z;
			shape_normals[in2] += normal.x;	shape_normals[in2 + 1] += normal.y;	shape_normals[in2 + 2] += normal.z;
		}
		// Normalize each of the new normal vectors.
		for (unsigned i = 0; i < shape_normals.size(); i += 3)
		{
			glm::vec3 vec(shape_normals[i], shape_normals[i + 1], shape_normals[i + 2]);
			vec = glm::normalize(vec);
			shape_normals[i] = vec.x; shape_normals[i + 1] = vec.y; shape_normals[i + 2] = vec.z;
		}
	}
	void SetMat(GLfloat strength, GLfloat shininess)
	{
		shape_mat.specularStrength = strength;
		shape_mat.shininess = shininess;
	}
};

struct Plane : public Shape // Vertical plane of 1x1 units across.
{
	Plane()
	{
		shape_indices = {
			0, 1, 2,
			2, 3, 0
		};
		shape_vertices = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		shape_uvs = {
			0.0f, 0.0f,
			3.0f, 0.0f,
			3.0f, 3.0f,
			0.0f, 3.0f
		};

		ColorShape(1.0f, 1.0f, 1.0f);
		CalcAverageNormals(shape_indices, shape_indices.size(), shape_vertices, shape_vertices.size());
	}
};

struct Grid : public Shape // Flat grid on ground. Starts at 0,0,0 and increases into positive.
{
	Grid(int quads, int scale = 1)
	{
		for (int row = 0; row <= quads; row++)
		{
			for (int col = 0; col <= quads; col++)
			{
				shape_vertices.push_back(col);
				shape_vertices.push_back(row);
				shape_vertices.push_back(0.0f);
			}
		}

		int i = 0;
		for (int row = 0; row < (quads * (quads + 1)); row = i * (quads + 1))
		{
			if (i % 2 == 0) // Even row: l->r
			{
				for (int col = 0; col < quads; col++)
				{
					// Triangle one.
					shape_indices.push_back(row + col);
					shape_indices.push_back(row + (col + 1));
					shape_indices.push_back(row + (quads + 1) + (col + 1));
					// Triangle two.
					shape_indices.push_back(row + (quads + 1) + (col + 1));
					shape_indices.push_back(row + (quads + 1) + col);
					shape_indices.push_back(row + col);
				}
			}
			else // Odd row: r->l
			{
				for (int col = quads - 1; col >= 0; col--)
				{
					// Triangle one.
					shape_indices.push_back(row + col);
					shape_indices.push_back(row + (col + 1));
					shape_indices.push_back(row + (quads + 1) + (col + 1));
					// Triangle two.
					shape_indices.push_back(row + (quads + 1) + (col + 1));
					shape_indices.push_back(row + (quads + 1) + col);
					shape_indices.push_back(row + col);
				}
			}
			i++;
		}

		for (int i = 0; i < shape_vertices.size(); i += 3)
		{
			shape_uvs.push_back(shape_vertices[i] / quads * scale);
			shape_uvs.push_back(shape_vertices[i + 1] / quads * scale);
		}
		ColorShape(1.0f, 1.0f, 1.0f);
		CalcAverageNormals(shape_indices, shape_indices.size(), shape_vertices, shape_vertices.size());
	}
};
struct Cube : public Shape
{
	Cube(int scale = 1)
	{
		shape_indices = {
			// Front.
			0, 1, 2,
			2, 3, 0,
			// Right.
			4, 5, 6,
			6, 7, 4,
			// Back.
			8, 9, 10,
			10, 11, 8,
			// Left.
			12, 13, 14,
			14, 15, 12,
			// Top.
			16, 17, 18,
			18, 19, 16,
			// Bottom.
			20, 21, 22,
			22, 23, 20
		};
		shape_vertices = {
			// Front.
			0.0f, 0.0f, 1.0f,		// 0.
			1.0f, 0.0f, 1.0f,		// 1.
			1.0f, 1.0f, 1.0f,		// 2.
			0.0f, 1.0f, 1.0f,		// 3.
			// Right.
			1.0f, 0.0f, 1.0f,		// 1. 4
			1.0f, 0.0f, 0.0f,		// 5. 5
			1.0f, 1.0f, 0.0f,		// 6. 6
			1.0f, 1.0f, 1.0f,		// 2. 7
			// Back.
			1.0f, 0.0f, 0.0f,		// 5. 8
			0.0f, 0.0f, 0.0f,		// 4. 9
			0.0f, 1.0f, 0.0f,		// 7. 10
			1.0f, 1.0f, 0.0f,		// 6. 11
			// Left.
			0.0f, 0.0f, 0.0f,		// 4. 12
			0.0f, 0.0f, 1.0f,		// 0. 13
			0.0f, 1.0f, 1.0f,		// 3. 14
			0.0f, 1.0f, 0.0f,		// 7. 15
			// Top.
			0.0f, 1.0f, 0.0f,		// 7. 16
			0.0f, 1.0f, 1.0f,		// 3. 17
			1.0f, 1.0f, 1.0f,		// 2. 18
			1.0f, 1.0f, 0.0f,		// 6. 19
			// Bottom.
			0.0f, 0.0f, 0.0f,		// 4. 20
			1.0f, 0.0f, 0.0f,		// 5. 21
			1.0f, 0.0f, 1.0f,		// 1. 22
			0.0f, 0.0f, 1.0f		// 0. 23
		};
		shape_uvs = {
			// Front.
			0.0f, 0.0f, 	// 0.
			1.0f, 0.0f, 	// 1.
			1.0f, 1.0f, 	// 2.
			0.0f, 1.0f,		// 3.
			// Right.
			0.0f, 0.0f, 	// 1.
			1.0f, 0.0f, 	// 5.
			1.0f, 1.0f, 	// 6.
			0.0f, 1.0f,		// 2.
			// Back.
			0.0f, 0.0f, 	// 5.
			1.0f, 0.0f, 	// 4.
			1.0f, 1.0f,		// 7.
			0.0f, 1.0f,		// 6.
			// Left.
			0.0f, 0.0f,		// 4.
			1.0f, 0.0f,		// 0.
			1.0f, 1.0f,		// 3.
			0.0f, 1.0f,		// 7.
			// Top.
			0.0f, 0.0f,		// 7.
			1.0f, 0.0f,		// 3.
			1.0f, 1.0f,		// 2.
			0.0f, 1.0f,		// 6.
			// Bottom.
			0.0f, 0.0f,		// 4.
			1.0f, 0.0f,		// 5.
			1.0f, 1.0f,		// 1.
			0.0f, 1.0f		// 0.
		};
		for (unsigned i = 0; i < shape_uvs.size(); i++)
			shape_uvs[i] *= scale;
		ColorShape(1.0f, 1.0f, 1.0f);
		CalcAverageNormals(shape_indices, shape_indices.size(), shape_vertices, shape_vertices.size());
	}
};

struct Prism : public Shape
{
	Prism(int sides)
	{
		float theta = 0.0f;
		// Top face.
		shape_vertices.push_back(0.5f);
		shape_vertices.push_back(1.0f);
		shape_vertices.push_back(0.5f);
		for (int i = 0; i < sides; ++i)
		{
			shape_vertices.push_back(0.5f + 0.5f * cos(theta));
			shape_vertices.push_back(1.0f);
			shape_vertices.push_back(0.5f + 0.5f * sin(theta));
			theta += 2 * PI / sides;
		}
		// Bottom face.
		shape_vertices.push_back(0.5f);
		shape_vertices.push_back(0.0f);
		shape_vertices.push_back(0.5f);
		for (int i = 0; i < sides; ++i)
		{
			shape_vertices.push_back(0.5f + 0.5f * cos(theta));
			shape_vertices.push_back(0.0f);
			shape_vertices.push_back(0.5f + 0.5f * sin(theta));
			theta += 2 * PI / sides;
		}
		// Indices now.
		// Bottom face.
		for (int i = sides + 1; i < sides * 2; i++)
		{
			shape_indices.push_back(sides + 1);
			shape_indices.push_back(i + 1);
			shape_indices.push_back(i + 2);
		}
		shape_indices.push_back(sides + 1);
		shape_indices.push_back(sides * 2 + 1);
		shape_indices.push_back(sides + 2);
		// Middle faces.
		for (int i = 1; i < sides; i++)
		{
			// Triangle one.
			shape_indices.push_back(i);
			shape_indices.push_back(i + 1);
			shape_indices.push_back(sides + i + 2);
			// Triangle two.
			shape_indices.push_back(sides + i + 2);
			shape_indices.push_back(sides + i + 1);
			shape_indices.push_back(i);
		}
		shape_indices.push_back(sides);
		shape_indices.push_back(1);
		shape_indices.push_back(sides + 2);
		shape_indices.push_back(sides + 2);
		shape_indices.push_back(sides * 2 + 1);
		shape_indices.push_back(sides);
		// Top face.
		for (int i = 1; i < sides; i++)
		{
			shape_indices.push_back(0);
			shape_indices.push_back(i + 1);
			shape_indices.push_back(i);
		}
		shape_indices.push_back(0);
		shape_indices.push_back(1);
		shape_indices.push_back(sides);
		for (int i = 0; i < shape_vertices.size(); i += 3)
		{
			shape_uvs.push_back(0); // No texture for grid so value doesn't matter.
			shape_uvs.push_back(0);
		}
		ColorShape(1.0f, 1.0f, 1.0f);
		CalcAverageNormals(shape_indices, shape_indices.size(), shape_vertices, shape_vertices.size());
	}
};

struct Cone : public Shape
{
	Cone(int sides)
	{
		float theta = 0.0f;
		// Bottom face.
		shape_vertices.push_back(0.5f);
		shape_vertices.push_back(0.0f);
		shape_vertices.push_back(0.5f);
		for (int i = 0; i < sides; ++i)
		{
			shape_vertices.push_back(0.5f + 0.5f * cos(theta));
			shape_vertices.push_back(0.0f);
			shape_vertices.push_back(0.5f + 0.5f * sin(theta));
			theta += 2 * PI / sides;
		}
		shape_vertices.push_back(0.5f);
		shape_vertices.push_back(1.0f);
		shape_vertices.push_back(0.5f);
		// Indices now. Bottom face.
		for (int i = 1; i < sides; i++)
		{
			shape_indices.push_back(0);
			shape_indices.push_back(i);
			shape_indices.push_back(i + 1);
		}
		shape_indices.push_back(0);
		shape_indices.push_back(sides);
		shape_indices.push_back(1);
		// Middle faces.
		for (int i = 1; i < sides; i++)
		{
			shape_indices.push_back(i);
			shape_indices.push_back(sides + 1);
			shape_indices.push_back(i + 1);
		}
		shape_indices.push_back(sides);
		shape_indices.push_back(sides + 1);
		shape_indices.push_back(1);

		for (int i = 0; i < shape_vertices.size(); i += 8)
		{

			/*shape_uvs = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
			};*/

			shape_uvs.push_back(0.0f);
			shape_uvs.push_back(0.0f);
			shape_uvs.push_back(1.0f);
			shape_uvs.push_back(0.0f);
			shape_uvs.push_back(1.0f);
			shape_uvs.push_back(1.0f);
			shape_uvs.push_back(0.0f);
			shape_uvs.push_back(1.0f);
		}

		ColorShape(1.0f, 1.0f, 1.0f);
		CalcAverageNormals(shape_indices, shape_indices.size(), shape_vertices, shape_vertices.size());
	}
};