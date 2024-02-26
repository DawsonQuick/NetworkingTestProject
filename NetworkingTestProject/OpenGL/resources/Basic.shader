#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texture;
layout (location = 2) in vec3 translation; // Assuming translation is a 3D vector

out vec3 v_TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 translationMatrix = mat4(1.0);
    translationMatrix[3] = vec4(translation, 1.0);

    // Apply transformations: translation, view, and projection
    gl_Position = projection * view * translationMatrix * vec4(position, 1.0);

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