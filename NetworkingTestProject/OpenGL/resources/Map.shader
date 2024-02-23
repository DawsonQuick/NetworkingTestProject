#shader vertex
#version 400 core

layout (location = 0) in vec3 position;
uniform mat4 translation;
uniform mat4 projection;
uniform mat4 view;

out vec4 fragPosition;
void main()
{
	gl_Position = projection*view *translation * vec4(position,1.0);
	fragPosition = translation*vec4(position,1.0);
};

#shader fragment
#version 400 core

layout (location = 0) out vec4 color;
out vec4 FragColor;
uniform vec3 lightPos[100];
uniform int numberOfLights;
uniform int isDynamicLightingOn;
in vec4 fragPosition;

void main()
{
	
	float normal = 255.0;
	vec4 textureColor = vec4((24/normal), (190/normal), (245/normal), 1.0);

	if(isDynamicLightingOn == 1){
		float maxLightDistance = 25.0;
		float total=0.0;
		
		float ambiantLighting = 25/normal;
		

		for(int i=0; i<numberOfLights; ++i){	

			float distance = sqrt((pow(lightPos[i].x-fragPosition.x,2))+(pow(lightPos[i].y-fragPosition.y,2)));
			if(distance >= maxLightDistance){
				//If the distance exceeds the effective range of the light it then has no effect on the total
				total += 0.0;
			}
			else{
				//Preform Inverse Square Law
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
			FragColor = min(max(vec4(textureColor.r*total,textureColor.g*total,textureColor.b*total,1.0),vec4(textureColor.r*ambiantLighting,textureColor.g*ambiantLighting,textureColor.b*ambiantLighting,1.0)),vec4(1.0,1.0,1.0,1.0));
		}
	}else{
		FragColor = textureColor;
	}
	
   
}