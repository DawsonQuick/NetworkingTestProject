#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 u_color;
out vec4 inColor;
void main()
{
	gl_Position = projection*view *translation * vec4(position,1.0);
	inColor = u_color;
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