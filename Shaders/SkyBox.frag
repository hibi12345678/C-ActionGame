#version 330 core
out vec4 FragColor;

in vec3 TexCoords;  // ���_�V�F�[�_�[����n���ꂽ���W

uniform samplerCube skybox;  // Cubemap �e�N�X�`��

void main()
{  
    FragColor = texture(skybox, TexCoords);  // Cubemap �e�N�X�`�����g�p
}