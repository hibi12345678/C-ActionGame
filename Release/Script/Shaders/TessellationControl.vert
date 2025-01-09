#version 330 core


layout(vertices = 4) out; // 1つのパッチ内の出力制御点数

in vec2 vTexCoord[]; // 入力 UV 座標 (頂点シェーダーから)
out vec2 tcTexCoord[]; // 出力 UV 座標 (TESに渡す)

void main()
{
    // 制御点データをそのまま渡す
    tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // テッセレーションの外側・内側のレベルを設定
    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = 5.0; // テッセレーション外周のレベル
        gl_TessLevelOuter[1] = 5.0;
        gl_TessLevelOuter[2] = 5.0;
        gl_TessLevelOuter[3] = 5.0;
        gl_TessLevelInner[0] = 5.0; // テッセレーション内部のレベル
        gl_TessLevelInner[1] = 5.0;
    }
}