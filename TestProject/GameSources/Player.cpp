/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		ObjectBase(StagePtr),
		m_cntlNum(0),
		m_Speed(3.0f),
		m_Jumpforce(4.0f),
		m_Dethtime(0.0f),
		m_Jumpjudge(false),
		m_Dethtimejudge(false)
	{}
	


	void Player::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(0.0f, 0.5f, 0.0);
		ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTransform->SetScale(0.125f, 0.125f, 0.125);

		auto ptrColl = AddComponent<CollisionObb>();

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		auto ptrGrav = AddComponent<Gravity>();
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);


		auto Shadowptr = AddComponent<Shadowmap>();
		Shadowptr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	//�i�s�x�N�g���ւ̉����֐�
	Vec3 Player::GetMoveVector() const{
		Vec3 angle(0, 0, 0);
		float moveX = m_cntl.LX;
		float moveZ = m_cntl.LY;
		
		float moveLength = 0;   //���������̃X�s�[�h
		auto ptrTransform = GetComponent<Transform>();
		auto ptrCamera = OnGetDrawCamera();
		//�i�s�����̌������v�Z
		auto front = ptrTransform->GetWorldPosition() - ptrCamera->GetEye();
		front.y = 0;
		front.normalize();
		//�i�s���������̊p�x���Z�o
		float frontAngle = atan2(front.z, front.x);
		//�R���g���[���̌����v�Z

		Vec2 moveVec(moveX, moveZ);
		float moveSize = moveVec.length();
		//�R���g���[���[�̌�������p�x���v�Z
		float cntlAngle = atan2(-moveX, moveZ);
		//�g�[�^���̊p�x���Z�o
		float totalAngle = frontAngle + cntlAngle;
		//�p�x����x�N�g�����쐬
		angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
		//���K������
		angle.normalize();
		//�ړ��T�C�Y��ݒ�
		angle *= moveSize;
		//Y���͕ω������Ȃ�
		angle.y = 0;
		return angle;
	}

	void Player::Move() {
		auto elapsedtime = App::GetApp()->GetElapsedTime();
		m_PlayerAngle = GetMoveVector();
		if (m_PlayerAngle.length() > 0.0f && !m_Dethtimejudge) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += m_PlayerAngle * elapsedtime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos);
		}
		if (m_PlayerAngle.length() > 0.0f && !m_Dethtimejudge) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(m_PlayerAngle, 1.0f);
		}
		if (m_Dethtimejudge) {
			m_Dethtime += elapsedtime;
		}
		if (m_Dethtime >= 3.0f) {
			m_Dethtimejudge = false;
			m_Dethtime = 0.0f;
		}
	}

	void Player::OnPushA() {
		if (/*m_cntl.A && */m_Jumpjudge) {
			auto grav = GetComponent<Gravity>();
			grav->StartJump(Vec3(0.0f, m_Jumpforce, 0.0f));
			m_Jumpjudge = false;
		}
		//m_cntl.A = false;

	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Obj) {
		auto ptrTransform = GetComponent<Transform>();
		if (Obj->FindTag(L"Deth")) {
			ptrTransform->SetPosition(0.0f, 0.5f, 0.0f);
			ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
			m_Dethtimejudge = true;
		}
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj) {
			m_Jumpjudge = true;
		}
	}

	void Player::OnUpdate() {
		m_inputHandler.PushHandle(m_cntlNum, m_cntl);
		m_Handler.PushHandler(GetThis<Player>());
		Move();
	}


}
//end basecross

