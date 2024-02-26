#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texture;

out vec3 v_TexCoord;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 view;

void main()
{

    // Apply transformations: translation, view, and projection
    gl_Position = projection * view * translation * vec4(position, 1.0);

    // Pass texture coordinates to fragment shader
    v_TexCoord = texture;
}

#shader fragment
#version 400 core

layout (location = 0) out vec4 color;
in vec3 v_TexCoord;
out vec4 FragColor;
uniform sampler2D u_Texture;
void main()
{
	vec4 color = texture(u_Texture,v_TexCoord.xy);
	FragColor = vec4(color.r,color.g, color.b, color.a);
}