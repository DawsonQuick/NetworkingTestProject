#shader vertex
#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 view;
out vec2 TexCoords;
out vec4 fragPosition;
void main()
{
	gl_Position = projection*view *translation * vec4(position,1.0);
	fragPosition = translation*vec4(position,1.0);
	TexCoords = texCoord;
};

#shader fragment
#version 430 core

layout(binding = 0,std430) buffer ParticlesBuffer {
    float particles[];
};


out vec4 FragColor;
uniform int numberOfLights;
uniform int isDynamicLightingOn;
uniform sampler2D u_texture;
in vec4 fragPosition;
in vec2 TexCoords;
void main()
{
	
	float normal = 255.0;
	vec4 textureColor = texture(u_texture,TexCoords);

	if(isDynamicLightingOn == 1){
		float maxLightDistance = 2.0;
		float total=0.0;
		
		float ambiantLighting = 25/normal;
		

		for(int i=0; i<numberOfLights; i=i+3){	
			vec3 tmp = vec3(particles[i+0],particles[i+1],particles[i+2]);
			float distance = distance(tmp.xyz, fragPosition.xyz);
			if(distance <= maxLightDistance){
				total += 1.0 - pow(distance /(maxLightDistance), 2.0);
			}
		}

		//Set the minimun lighting to the ambiant lighting
		if(total < 0.001){
			FragColor = vec4(textureColor.r*ambiantLighting,textureColor.g*ambiantLighting,textureColor.b*ambiantLighting,1.0);
		}

		//The resulting out color with be the result of the background * the intensity calculated by the inverse square law
		//Clamped the minimum lighting to be that of the ambient and max to be 1.0(white)
		else{
			
			vec3 lightColor = vec3((255/normal), (0/normal), (0/normal));
			FragColor = min(max(vec4(((textureColor.r+lightColor.r)*total),((textureColor.g+lightColor.g)*total),((textureColor.b+lightColor.b)*total),1.0),vec4(textureColor.r*ambiantLighting,textureColor.g*ambiantLighting,textureColor.b*ambiantLighting,1.0)),vec4(1.0,1.0,1.0,1.0));
		}
	}else{
		FragColor = textureColor;
	}
	
   
}