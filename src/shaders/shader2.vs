#version 330 core
//location metadata about the position of the attribute
layout (location = 0) in vec3 position; //position attribute location is 0
layout (location = 1) in vec3 color;
out vec3 vertexColor;
void main()
{
   gl_Position = vec4(position.x, position.y, position.z, 1.0);
   vertexColor = position;
}
