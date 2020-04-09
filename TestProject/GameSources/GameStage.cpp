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
		const Vec3 eye(0.0f, 10.0f, -20.0f);
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

	void GameStage::GamePlayer() {
		auto playerptr = AddGameObject<Player>();
		SetSharedGameObject(L"Player", playerptr);
	}

	void GameStage::Object() {

	}
	void GameStage::OnCreate() {
		try {
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			GamePlayer();
		AddGameObject<FixBox>(
			Vec3(30.0f, 0.1f, 10.0f),
			Vec3(0.0f),
			Vec3(0.0f, -1.0f, 0.0f)
			);
		AddGameObject<FixBox>(
			Vec3(2.0f, 1.0f, 3.0f),
			Vec3(0.0f),
			Vec3(2.0f, -0.5f, 0.0f)
			);
		AddGameObject<ReturnCube>(
			Vec3(0.5f),
			Vec3(0.0f),
			Vec3(5.0f, -1.0f, 0.0f)
			);


			AddGameObject<DebugTest>();
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStageHurukawa::CreateViewLight() {
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

	void GameStageHurukawa::OnCreate() {
		try {
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
				//Floor�̒ǉ�
				AddGameObject<Floor2>
				(
					Vec3(50.0f, 1.0f, 50.0f),//sacle x,y,z
					Vec3(0.0f),//rotate x,y,z
					Vec3(0.0f, -0.5f, 0.0f)//position x,y,zd
					);
			auto gateptr = AddGameObject<Gate>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(-5.0f, 10.0f, -12.5f)//position x,y,zd
					);

			auto gate2ptr = AddGameObject<Gate2>
				(
					Vec3(1.0f, 20.0f, 25.0f),//sacle x,y,z
					Vec3(0.0f, 0, 0),//rotate x,y,z
					Vec3(-5.0f, 10.0f, 0.0f)//position x,y,zd
					);
			//Player�̒ǉ�
			AddGameObject<Playerhurukawa>
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
			AddGameObject<Button2>
				(
					Vec3(0.5f, 0.5f, 0.5f),
					Vec3(0.0f),
					Vec3(1.0f, 0.0f, 1.0f)
					);
			AddGameObject<Button22>
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


	//�M�_�̃X�e�[�W
	void GameStageShogo::CreateViewLight() {
		const Vec3 eye(0.0f, 10.0f, -20.0f);
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

	void GameStageShogo::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();

			//�����v�Z�L��
			SetPhysicsActive(true);

			//�i���j�v���C���[
			auto PlayerPtr = AddGameObject<TestPlayer>(Vec3(-7, 1, 0), Vec3(0), Vec3(1));
			//�v���C���[�̓o�^
			SetSharedGameObject(L"Player", PlayerPtr);

			//�Ђ�
			auto HimoPtr = AddGameObject<Himo>(Vec3(4, 5, 0), Vec3(0), Vec3(0.5, 2, 0.5));
			//�Ђ��̓o�^
			SetSharedGameObject(L"Himo", HimoPtr);
			//������
			auto OmoriPtr = AddGameObject<Omori>(Vec3(4, 3.5, 0), Vec3(0), Vec3(1));
			//������̓o�^
			SetSharedGameObject(L"Omori", OmoriPtr);
			//�M�_
			AddGameObject<HeatStick>(Vec3(0, 6, 0), Vec3(0), Vec3(10, 1, 1));

			//�{�^��
			AddGameObject<Button>(Vec3(4, 1, 0), Vec3(0), Vec3(2,1,2));

			//��(��)
			AddGameObject<Floor>(Vec3(-8, 0, 0), Vec3(0), Vec3(10, 1, 15));
			//��(�E)
			AddGameObject<Floor>(Vec3(8, 0, 0), Vec3(0), Vec3(10, 1, 15));
			//��(��)
			AddGameObject<Floor>(Vec3(0, -4, 0), Vec3(0), Vec3(20, 1, 15));

			//��(�i�i������)
			auto MoveFloorPtr1 = AddGameObject<MoveFloor>(Vec3(0, 0.01f, 5), Vec3(0, 0, -45), Vec3(8, 1, 3));
			//�������o�^
			SetSharedGameObject(L"MoveFloor1", MoveFloorPtr1);
			//��(���R����)
			auto MoveFloorPtr2 = AddGameObject<MoveFloor>(Vec3(8, -0.5f, -5), Vec3(0), Vec3(8, 1, 3));
			//�������o�^
			SetSharedGameObject(L"MoveFloor2", MoveFloorPtr2);

			//�����i�|�W�V�����A�X�P�[���j
			auto Fountain1 = AddGameObject<Fountain>(Vec3(0.0f, -3.0f, 0.0f), Vec3(1.0f));
			//����
			SetSharedGameObject(L"Fountain1", Fountain1);

		}
		catch (...) {
			throw;
		}
	}
}
//end basecross
