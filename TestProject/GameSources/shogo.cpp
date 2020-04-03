/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//�M�_
	void HeatStick::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//�M�_�̃G���A-5�A-1�A�O����|�W�V�����܂�
		m_HeatArea = AABB(Vec3(-5, -1, 0), m_Pos);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		//�Ђ��̓o�^
		m_himo = GetStage()->GetSharedGameObject<Himo>(L"Himo");
	}
	void HeatStick::OnUpdate()
	{
		//�v���C���[�̓o�^
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<TestPlayer>(L"Player");

		auto stage = GetStage();

		//������̓o�^
		auto ObjectPtr_O = GetStage()->GetSharedGameObject<Omori>(L"Omori");

		//���s���Ԍv��
		auto elapsedTime = App::GetApp()->GetElapsedTime();

		if (ObjectPtr_P) {
			//�v���C���[�̃|�W�V�������擾
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//�d��̏d��
			auto OmoriGrav = ObjectPtr_O->GetComponent<Gravity>();

			//�v���C���[�̒��S����P�C�P�C�P�͈̔�
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			//�v���C���[���M�_�͈̔͂ɓ�������
			if (HitTest::AABB_AABB(m_HeatArea, PlayerAABB)) {
				count++;
				//��T�b��ɏ���
				if (count >= 250) {
					if (m_himo) {
						//�Ђ�������
						stage->RemoveGameObject<GameObject>(m_himo->GetThis<GameObject>());
						//�d��̏d�͂��I���ɂ���
						OmoriGrav->SetUpdateActive(true);
					}
				}
			}
		}
	}
	//�i���j�v���C���[
	void TestPlayer::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);
		//�d��
		auto grav = AddComponent<Gravity>();
		//�^�O������
		AddTag(L"Player");

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
	}
	void TestPlayer::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();
		//���s���Ԍv��		
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		//�L�[�{�[�h�̎擾(�L�[�{�[�h�D��)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) {
			//�O
			pos.z += 1.0f * elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['A']) {
			//��
			pos.x += -1.0f* elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['S']) {
			//���
			pos.z += -1.0f* elapsedTime;
		}
		if (KeyState.m_bPushKeyTbl['D']) {
			//�E
			pos.x += 1.0f* elapsedTime;
		}
		if (KeyState.m_bLastKeyTbl[VK_SPACE]) {
			//��
			pos.y += 5.0f* elapsedTime;
		}
		if (KeyState.m_bLastKeyTbl[VK_LSHIFT]) {
			//��
			pos.y += -1.0f* elapsedTime;
		}
		transComp->SetPosition(pos);

	}
	//�Ђ�
	void Himo::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	//�d��
	void Omori::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();

		//�d��
		auto grav = AddComponent<Gravity>();
		//�����ɂ��Ă���
		grav->SetUpdateActive(false);

		//�^�O
		AddTag(L"Omori");
	}
	//�{�^��
	void Button::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
	}
	void Button::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"Omori")) {
			//������
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor1");
			auto transComp = ObjectPtr_MF->GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();
			//���s���Ԍv��		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			ObjectPtr_MF->SetActive(true);
		}
	}
	void Button::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"Omori")) {
			//������
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor2");
			auto transComp = ObjectPtr_MF->GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();
			//���s���Ԍv��		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			if (pos.x > 0) {
				pos.x += -2.0f * elapsedTime;
				transComp->SetPosition(pos);
			}
		}
	}

	//��
	void Floor::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	//������
	void MoveFloor::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_Pos);
		transComp->SetScale(m_Scale);
		transComp->SetRotation(m_Rotation);
		//���_�̍��W��ύX
		transComp->SetPivot(3, 0.1f, 5);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}
	void MoveFloor::OnUpdate()
	{
		if (m_Active)
		{
			m_Time += App::GetApp()->GetElapsedTime();
			if (m_Time > 5.0f)
				m_Active = false;
			auto ptrTrans = GetComponent<Transform>();
			m_Rotation = ptrTrans->GetRotation();
			Easing<Vec3> easing;
			auto Force = easing.Linear(m_Rotation, Vec3(0, 0, 0), m_Time, 5.0f);
			const float LerpFact = 1.0f;
			bsm::Quat FQuat;
			FQuat.rotationRollPitchYawFromVector(Force);
			FQuat.normalize();
			ptrTrans->SetQuaternion(FQuat);
		}
	}
}
//end basecross
