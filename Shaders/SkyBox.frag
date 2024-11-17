#version 330 core
out vec4 FragColor;

in vec3 TexCoords;  // 頂点シェーダーから渡された座標

uniform samplerCube skybox;  // Cubemap テクスチャ

void main()
{  
    FragColor = texture(skybox, TexCoords);  // Cubemap テクスチャを使用
}