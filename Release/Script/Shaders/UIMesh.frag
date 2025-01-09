#version 330

// ���_�V�F�[�_�[����̓���
in vec2 fragTexCoord;  // �e�N�X�`�����W

// �o��
out vec4 outColor;  // �`�悳���ŏI�I�ȐF

// ���j�t�H�[��
uniform sampler2D uTexture;  // �e�N�X�`���T���v���[

void main()
{
    // �e�N�X�`���J���[���擾
    vec4 texColor = texture(uTexture, fragTexCoord);

    // �o�͐F��ݒ�
    outColor = texColor;
}