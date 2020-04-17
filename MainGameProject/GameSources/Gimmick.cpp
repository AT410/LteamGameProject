/*!
@breif	�M�~�b�N�֘A����
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	///�X�C�b�`�I�u�W�F�N�g
	void SwitchObj::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::Auto);
	}

	void SwitchObj::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"EnabledSwitch")) {
			//�X�C�b�`�������ꂽ���̔���
			//GetStage()->RemoveGameObject<GameObject>(Other);
			auto ObjectPtr_MF = GetStage()->GetSharedGameObject<MoveFloor>(L"MoveFloor");
			ObjectPtr_MF->SetActive(true);
		}
	}

	void SwitchObj::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		//Omori�^�O������������ƏՓ˂���
		if (Other->FindTag(L"EnabledSwitch")) {
			//���R������
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

	void SwitchObj::OnCollisionExit(shared_ptr<GameObject>& Other) {
		if (!m_IsKeep)
			return;
		if (Other->FindTag(L"EnabledSwitch")) {
			m_Active = m_IsKeep ? false : m_Active;
		}
	}


	//�M�_
	void HeatStick::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//�M�_�̃G���A-5�A-1�A�O����|�W�V�����܂�
		//�ω\�ɏC��
		m_HeatArea = AABB(Vec3(-3, -1, 0), m_pos);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
	}


	void HeatStick::OnUpdate()
	{
		if (m_player) {
			//�v���C���[�̃|�W�V�������擾
			auto PlayerPos = m_player->GetComponent<Transform>()->GetPosition();

			//�v���C���[�̒��S����P�C�P�C�P�͈̔�
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			//�v���C���[���M�_�͈̔͂ɓ�������
			if (HitTest::AABB_AABB(m_HeatArea, PlayerAABB)) {
				//���s���Ԍv��
				auto elapsedTime = App::GetApp()->GetElapsedTime();

				m_count += elapsedTime;
				//��T�b��ɏ���
				if (m_count >= 5.0f) {
					if (m_himo) {
						//�Ђ�������
						GetStage()->RemoveGameObject<GameObject>(m_himo->GetThis<GameObject>());
						//�d��̏d�͂��I���ɂ���
					//�d��̏d��
					//������̓o�^
					auto ObjectPtr_O = GetStage()->GetSharedGameObject<ObjectBase>(L"Omori");

					auto OmoriGrav = ObjectPtr_O->GetComponent<Gravity>();
						OmoriGrav->SetUpdateActive(true);
					}
				}
			}
		}
		else
		{
			//�Ђ��̓o�^
			m_himo = GetStage()->GetSharedGameObject<FixedObj>(L"Himo");
			m_player = GetStage()->GetSharedGameObject<Player>(L"Player");
		}
	}


	//�d��
	void Omori::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetAfterCollision(AfterCollision::None);

		//�d��
		auto grav = AddComponent<Gravity>();
		//�����ɂ��Ă���
		grav->SetUpdateActive(false);

		//�^�O
		for (auto Tag : m_tag) {
			AddTag(Tag);
		}

		//AddTag(L"EnabledSwitch");
		//���L�ݒ�
		GetStage()->SetSharedGameObject(L"Omori", GetThis<Omori>());
	}

	void Omori::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		GetComponent<Gravity>()->SetGravityZero();
	}


	//������
	void MoveFloor::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(m_meshKey);
		drawComp->SetTextureResource(m_texKey);

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);
		transComp->SetQuaternion(Quat(m_rot));
		//���_�̍��W��ύX
		transComp->SetPivot(3, 0.1f, 5);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);

		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<MoveFloor>());
		}
	}
	void MoveFloor::OnUpdate()
	{
		if (m_Active)
		{
			auto transComp = GetComponent<Transform>();
			Vec3 pos = transComp->GetPosition();
			Vec3 rot = transComp->GetRotation();

			//���s���Ԍv��		
			auto elapsedTime = App::GetApp()->GetElapsedTime();

			if (pos.x > 0) {
				pos.x += -2.0f * elapsedTime;
				transComp->SetPosition(pos);
			}

			//m_Time += App::GetApp()->GetElapsedTime();
			//if (m_Time > 5.0f) {
			//	m_Active = false;
			//}
			//auto ptrTrans = GetComponent<Transform>();
			//auto Rotation = ptrTrans->GetRotation();
			//Easing<Vec3> easing;
			//auto Force = easing.Linear(Rotation, Vec3(0, 0, 0), m_Time, 5.0f);
			//const float LerpFact = 1.0f;
			//bsm::Quat FQuat;
			//FQuat.rotationRollPitchYawFromVector(Force);
			//FQuat.normalize();
			//ptrTrans->SetQuaternion(FQuat);
		}
	}


	//����
	void Fountain::OnCreate()
	{
		//�`��
		auto drawComp = AddComponent<BcPNTnTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");

		//�|�W�V�����A�X�P�[���A��]
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_pos);
		transComp->SetScale(m_scal);

		//�R���W������t����
		auto ptrColl = AddComponent<CollisionObb>();
		//ptrColl->SetFixed(true);

		//�d��
		auto grav = AddComponent<Gravity>();

		//�����G���A����|�W�V�����܂�
		m_FountainArea = AABB(Vec3(0, -2, 0), 6, 1, 14);
		m_FountainArea2 = AABB(Vec3(-1, -1, -3),Vec3(1, 4, 0));

	}

	void Fountain::OnUpdate()
	{
		auto transComp = GetComponent<Transform>();
		Vec3 pos = transComp->GetPosition();

		//�v���C���[�̓o�^
		auto ObjectPtr_P = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto stage = GetStage();

		if (ObjectPtr_P) {
			//�v���C���[�̃|�W�V�������擾
			auto PlayerPos = ObjectPtr_P->GetComponent<Transform>()->GetPosition();
			//�v���C���[�̒��S����P�C�P�C�P�͈̔�
			AABB PlayerAABB = AABB(PlayerPos, 1, 1, 1);
			if (HitTest::AABB_AABB(m_FountainArea, PlayerAABB)) {
				//�v���C���[�̃|�W�V�������X�^�[�g��
				ObjectPtr_P->ResetPositon();

				//�v���C���[������
				//ObjectPtr_P->SetDrawActive(false);
			}
			if (m_Active) {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 3.5f) {
					m_Active = false;
					AddComponent<Gravity>()->StartJump(Vec3(0, 12.5f, 0.0f));
					m_Time = 0;
				}
				else
				{
					if (pos.y < 0)
						return;
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//�v���C���[�̃|�W�V�������X�^�[�g��
						ObjectPtr_P->ResetPositon();

						//�v���C���[������
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
			else {
				m_Time += App::GetApp()->GetElapsedTime();
				if (m_Time > 1.5f) {
					if (HitTest::AABB_AABB(m_FountainArea2, PlayerAABB)) {
						//�v���C���[�̃|�W�V�������X�^�[�g��
						ObjectPtr_P->ResetPositon();

						//�v���C���[������
						//ObjectPtr_P->SetDrawActive(false);
					}
				}
			}
		}
	}

	void Fountain::OnCollisionEnter(shared_ptr<GameObject>& Other) {

		m_Active = true;
	}

}