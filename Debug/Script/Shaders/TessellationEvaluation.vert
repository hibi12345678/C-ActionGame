#version 420 core

layout(quads, fractional_odd_spacing, cw) in;

in vec2 tcTexCoord[]; // TCS から受け取る UV 座標
out vec2 teTexCoord;  // フラグメントシェーダーに渡す UV 座標

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
    // テッセレーション後の位置を補間
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // 双線形補間で位置を計算
    vec3 pos = mix(mix(p0, p1, u), mix(p3, p2, u), v);

    // UV座標も補間
    vec2 uv0 = tcTexCoord[0];
    vec2 uv1 = tcTexCoord[1];
    vec2 uv2 = tcTexCoord[2];
    vec2 uv3 = tcTexCoord[3];
    teTexCoord = mix(mix(uv0, uv1, u), mix(uv3, uv2, u), v);

    // MVP行列を適用して出力位置を設定
    gl_Position = uViewProjection * uModel * vec4(pos, 1.0);
}