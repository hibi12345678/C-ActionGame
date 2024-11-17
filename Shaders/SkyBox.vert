#version 330 core
layout (location = 0) in vec3 aPos;  // 頂点の位置

out vec3 TexCoords;  // フラグメントシェーダーに渡す座標

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;  // 頂点の位置をそのまま渡す
    gl_Position =  projection * view * vec4(aPos, 1.0);  // モデルビュー行列とプロジェクション行列で変換
}