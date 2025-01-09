
#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 uViewProjection; // �J�����̃r���[�E�v���W�F�N�V�����s��

void main() {
    gl_Position = uViewProjection * vec4(aPos, 1.0);
}