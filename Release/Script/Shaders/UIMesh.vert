#version 330

// 頂点入力
layout(location = 0) in vec3 inPosition; 
layout(location = 1) in vec3 inNormal;   
layout(location = 2) in vec2 inTexCoord; 

// 出力（フラグメントシェーダーへの渡し）
out vec2 fragTexCoord;  // テクスチャ座標

// ユニフォーム
uniform mat4 uModelMatrix;      // モデル行列
uniform mat4 uViewProj;         // ビュー・プロジェクション行列

void main()
{
    vec4 worldPos = uModelMatrix * vec4(inPosition, 1.0);
    vec4 clipPos = uViewProj * worldPos;

    // 深度値を最小値（手前）に固定
    clipPos.z = clipPos.w * 0.0;

    gl_Position = clipPos;
    fragTexCoord = inTexCoord;
}