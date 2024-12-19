#version 420 core

layout(location = 0) in vec3 aPosition;   
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 aTexCoord;   

out vec2 teTexCoord; 
out vec3 fragWorldPos;
out vec3 fragNormal;

uniform mat4 uModel;           
uniform mat4 uViewProjection; 
uniform float uvScale;

void main()
{

	vec4 pos = uModel * vec4(aPosition, 1.0);
	fragWorldPos = pos.xyz;
    gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
	fragNormal = (vec4(inNormal, 0.0f)).xyz;
    teTexCoord = aTexCoord * uvScale;
}
