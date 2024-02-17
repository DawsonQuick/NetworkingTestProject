#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 view;
void main()
{
	gl_Position = projection*view *translation * vec4(position,1.0);
};

#shader fragment
#version 400 core

layout (location = 0) out vec4 color;

out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}