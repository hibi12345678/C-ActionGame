#version 420 core

in vec2 teTexCoord; 
in vec3 fragWorldPos;
in vec3 fragNormal;
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// This is used for the texture sampling
uniform sampler2D texture1;

void main()
{

    vec2 flippedTexCoord = vec2(teTexCoord.x, teTexCoord.y);
    outNormal = fragNormal;
    outDiffuse = texture(texture1, flippedTexCoord).xyz;   
    outWorldPos = fragWorldPos;
   
}