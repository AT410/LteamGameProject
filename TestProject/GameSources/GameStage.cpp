/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -60.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStage::OnCreate() {
		try {
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//Floor�̒ǉ�
			AddGameObject<Floor>
				(
					Vec3(50.0f, 1.0f, 50.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(0.0f, -0.5f, 0.0f)//position x,y,zd
					);
			auto gateptr = AddGameObject<Gate>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f,0,0),//rotate x,y,z
					Vec3(-5.0f, 10.0f, -12.5f)//position x,y,zd
					);

			auto gate2ptr = AddGameObject<Gate2>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(-5.0f,10.0f, 0.0f)//position x,y,zd
					);
			//Player�̒ǉ�
			AddGameObject<Player>
				(
					Vec3(1.0f, 1.0f, 1.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(-20.0f, +0.5f, 0.0f)//position x,y,z
					);

			auto gimmickptr = AddGameObject<Gimmick>
				(
					Vec3(4.0f, 1.0f, 3.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(10.0f, 1.0f, 0.0f)//position x,y,zd
					);
			SetSharedGameObject(L"gimmickobj", gimmickptr);
			SetSharedGameObject(L"gate", gateptr);
			SetSharedGameObject(L"gate2", gate2ptr);
			AddGameObject<Button>
				(
					Vec3(0.5f, 0.5f, 0.5f),
					Vec3(0.0f),
					Vec3(1.0f, 0.0f, 1.0f)
					);
			AddGameObject<Button2>
				(
					Vec3(0.5f, 0.5f, 0.5f),
					Vec3(0.0f),
					Vec3(-10.0f, 0.0f, 1.0f)
					);
			AddGameObject<Background>
			(
					Vec3(50.0f, 50.0f, 0.5f),
					Vec3(0.0f),
					Vec3(0.0f, 10.0f, 10.0f)
					);

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
