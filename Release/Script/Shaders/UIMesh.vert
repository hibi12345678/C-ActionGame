#version 330

// ���_����
layout(location = 0) in vec3 inPosition; 
layout(location = 1) in vec3 inNormal;   
layout(location = 2) in vec2 inTexCoord; 

// �o�́i�t���O�����g�V�F�[�_�[�ւ̓n���j
out vec2 fragTexCoord;  // �e�N�X�`�����W

// ���j�t�H�[��
uniform mat4 uModelMatrix;      // ���f���s��
uniform mat4 uViewProj;         // �r���[�E�v���W�F�N�V�����s��

void main()
{
    vec4 worldPos = uModelMatrix * vec4(inPosition, 1.0);
    vec4 clipPos = uViewProj * worldPos;

    // �[�x�l���ŏ��l�i��O�j�ɌŒ�
    clipPos.z = clipPos.w * 0.0;

    gl_Position = clipPos;
    fragTexCoord = inTexCoord;
}