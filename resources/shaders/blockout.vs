#version 430 core


//Source 6 (See readme for full referencing)


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal; 
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int overlayvs = 0;

void main(){
	if(overlayvs == 0){
		FragPos = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;
		TexCoords = aTexCoords;

		gl_Position = projection * view * vec4(FragPos, 1.0);
	}
	else{
		FragPos = vec3(1.0, 1.0, 1.0);
		TexCoords = aTexCoords;
		gl_Position = vec4(aPos.x / 10.0, aPos.y / 10.0, aPos.z, 1.0);
	}
}  