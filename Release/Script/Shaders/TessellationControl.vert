#version 330 core


layout(vertices = 4) out; // 1�̃p�b�`���̏o�͐���_��

in vec2 vTexCoord[]; // ���� UV ���W (���_�V�F�[�_�[����)
out vec2 tcTexCoord[]; // �o�� UV ���W (TES�ɓn��)

void main()
{
    // ����_�f�[�^�����̂܂ܓn��
    tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // �e�b�Z���[�V�����̊O���E�����̃��x����ݒ�
    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = 5.0; // �e�b�Z���[�V�����O���̃��x��
        gl_TessLevelOuter[1] = 5.0;
        gl_TessLevelOuter[2] = 5.0;
        gl_TessLevelOuter[3] = 5.0;
        gl_TessLevelInner[0] = 5.0; // �e�b�Z���[�V���������̃��x��
        gl_TessLevelInner[1] = 5.0;
    }
}