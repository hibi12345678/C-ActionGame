#version 420 core

in vec2 teTexCoord; 
in vec3 fragWorldPos;
in vec3 fragNormal;
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
void main()
{
    outNormal = fragNormal;
    float slope = fragNormal.z;  
    outWorldPos = fragWorldPos;
    float height = fragWorldPos.z;

    if (height >110.0) {
        
        outDiffuse = mix(texture(texture2, teTexCoord).xyz, texture(texture4, teTexCoord).xyz, smoothstep(0.0, 1.0, slope));
    } else if (height < -90.0) {
       
        outDiffuse = mix(texture(texture2, teTexCoord).xyz, texture(texture3, teTexCoord).xyz, smoothstep(0.0, 1.0, slope));
    } else {
       
        outDiffuse = mix(texture(texture2, teTexCoord).xyz, texture(texture1, teTexCoord).xyz, smoothstep(0.0, 1.0, slope));
    }
}
