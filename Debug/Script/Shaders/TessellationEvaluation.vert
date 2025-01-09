#version 420 core

layout(quads, fractional_odd_spacing, cw) in;

in vec2 tcTexCoord[]; // TCS ����󂯎�� UV ���W
out vec2 teTexCoord;  // �t���O�����g�V�F�[�_�[�ɓn�� UV ���W

uniform mat4 uModel;
uniform mat4 uViewProjection;

void main()
{
    // �e�b�Z���[�V������̈ʒu����
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // �o���`��Ԃňʒu���v�Z
    vec3 pos = mix(mix(p0, p1, u), mix(p3, p2, u), v);

    // UV���W�����
    vec2 uv0 = tcTexCoord[0];
    vec2 uv1 = tcTexCoord[1];
    vec2 uv2 = tcTexCoord[2];
    vec2 uv3 = tcTexCoord[3];
    teTexCoord = mix(mix(uv0, uv1, u), mix(uv3, uv2, u), v);

    // MVP�s���K�p���ďo�͈ʒu��ݒ�
    gl_Position = uViewProjection * uModel * vec4(pos, 1.0);
}