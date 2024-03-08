#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
uniform mat4 projection;
uniform mat4 view;
out vec4 inColor;
void main()
{
	gl_Position = projection*view * vec4(position,1.0);
	inColor = color;
};

#shader fragment
#version 400 core

layout (location = 0) out vec4 color;
out vec4 FragColor;
in vec4 inColor;
void main()
{
    FragColor = inColor;
}