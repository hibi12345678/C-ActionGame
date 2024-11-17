#version 330 core
layout (location = 0) in vec3 aPos;  // ���_�̈ʒu

out vec3 TexCoords;  // �t���O�����g�V�F�[�_�[�ɓn�����W

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;  // ���_�̈ʒu�����̂܂ܓn��
    gl_Position =  projection * view * vec4(aPos, 1.0);  // ���f���r���[�s��ƃv���W�F�N�V�����s��ŕϊ�
}