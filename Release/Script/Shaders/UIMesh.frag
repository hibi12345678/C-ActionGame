#version 330

// 頂点シェーダーからの入力
in vec2 fragTexCoord;  // テクスチャ座標

// 出力
out vec4 outColor;  // 描画される最終的な色

// ユニフォーム
uniform sampler2D uTexture;  // テクスチャサンプラー

void main()
{
    // テクスチャカラーを取得
    vec4 texColor = texture(uTexture, fragTexCoord);

    // 出力色を設定
    outColor = texColor;
}