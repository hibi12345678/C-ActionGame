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
uniform float uZScale;
void main()
{
    outNormal = fragNormal;
    float slope = fragNormal.z;  
    outWorldPos = fragWorldPos;
    float height = fragWorldPos.z;

    vec3 blendColor;

    if (height > 60.0 * uZScale) {
        float blendFactor = smoothstep(40.0 * uZScale, 70.0 * uZScale, height);
        blendColor = mix(texture(texture1, teTexCoord).xyz, 
                     texture(texture4, teTexCoord).xyz, 
                     blendFactor);
      outDiffuse = mix(texture(texture2, teTexCoord).xyz,blendColor,smoothstep(0.0, 1.0, slope));
    }
    
    else if (height < -90.0 * uZScale) {
        float blendFactor = smoothstep(-100.0 * uZScale, -80.0 * uZScale, height);
        blendColor = mix(texture(texture1, teTexCoord).xyz, 
                     texture(texture3, teTexCoord).xyz, 
                     blendFactor);
      outDiffuse = mix(texture(texture2, teTexCoord).xyz,blendColor,smoothstep(0.0, 1.0, slope));
    } 
    
    else {

        float blendFactor = smoothstep(-10.0 * uZScale, 10.0 * uZScale, height);
        outDiffuse = mix(texture(texture2, teTexCoord).xyz, texture(texture1, teTexCoord).xyz, smoothstep(0.0, 1.0, slope));
    }

}