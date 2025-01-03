//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Effect.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Effect class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      �R���X�g���N�^�ł�.
//-----------------------------------------------------------------------------
Effect::Effect() {
	manager = Effekseer::Manager::Create(8000);

	renderer = EffekseerRendererGL::Renderer::Create(8000);


	// �����_���[���}�l�[�W���ɃZ�b�g
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	// Effekseer��Renderer��OpenGL�ɃZ�b�g
	renderer->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovRH_OpenGL(45.0f, 800.0f / 600.0f, 1.0f, 100.0f));
	renderer->SetCameraMatrix(Effekseer::Matrix44().LookAtRH(Effekseer::Vector3D(10, 10, 10), Effekseer::Vector3D(0, 0, 0), Effekseer::Vector3D(0, 1, 0)));
	effect = Effekseer::Effect::Create(manager, "Assets/Effect/effect.efketc", 1.0f);
	// �G�t�F�N�g���Đ�
	int handle = manager->Play(effect, 0, 0, 0);


}

Effect::~Effect() {

	effect->Release();
	renderer->Release();
    manager->Release();

}

void Effect::DrawEffect() {

    //�L�������Ă镨��ޔ�
    glPushAttrib(GL_ENABLE_BIT);

    // Effekseer�̍X�V
    manager->Update();

    // OpenGL�̕`��ݒ�
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "GLError after glClear: " << error << std::endl;
    }

    // Effekseer�̕`��
    renderer->BeginRendering();
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "GLError after BeginRendering: " << error << std::endl;
    }

    manager->Draw();
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "GLError after manager->Draw: " << error << std::endl;
    }

    renderer->EndRendering();
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "GLError after EndRendering: " << error << std::endl;
    }


    //���ɖ߂�
    glPopAttrib();


    //�f�v�X�e�X�g������������Ă�̂ŗL����
    glEnable(GL_DEPTH_TEST);
}