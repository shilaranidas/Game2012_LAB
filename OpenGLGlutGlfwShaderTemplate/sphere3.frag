#version 430 core

in vec4 vColor;
out vec4 frag_color;

void main()
{
	frag_color = vColor;
}