/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//---------------------------------------------------
//�v���C���[�N���X�̎���
//---------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		ObjectBase(StagePtr),
		m_Speed(3.0f),
		m_Jumpforce(4.0f),
		m_StopActionTime(5.0f),
		m_Jumpjudge(false),
		m_StopActionTimeJudge(false)
	{}



	void Player::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetPosition(m_pos);
		ptrTransform->SetScale(m_scal);
		ptrTransform->SetQuaternion(Quat(m_rot));

		AddComponent<CollisionObb>();
		AddComponent<Gravity>();

		auto Shadowptr = AddComponent<Shadowmap>();
		Shadowptr->SetMeshResource(L"TESTN_MD");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"TESTN_MD");
		m_PlayerState = PlayerState::Start;
		
		//���L�o�^
		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto ptrGameStage = GetTypeStage<GameStage>();
		auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(ptrGameStage->GetMainView()->GetCamera());
		ptrMyCamera->SetTargetObject(GetThis<Player>());

		for (auto tag : m_tag)
		{
			AddTag(tag);
		}

		//�΂��Đ�
		auto EfkPoint = m_pos;
		EfkPoint.y += 0.5f;
		m_FireEfk = ObjectFactory::Create<EfkPlay>(L"FIRE_EFK", EfkPoint);

	}

	//�i�s�x�N�g���ւ̉����֐�
	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);

		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		float moveX = Cont.fThumbLX;
		float moveZ = Cont.fThumbLY;

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
		if (m_PlayerAngle.length() > 0.0f && !m_PushPull) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += m_PlayerAngle * elapsedtime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos);

			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(m_PlayerAngle, 1.0f);
		}
	}

	void Player::OnPushA() {
		if (m_Jumpjudge && !m_StopActionTimeJudge) {
			auto grav = GetComponent<Gravity>();
			grav->StartJump(Vec3(0.0f, m_Jumpforce, 0.0f));
			m_Jumpjudge = false;
			//GetTypeStage<GameStage>()->Effectplay(L"FIRE_EFK", GetComponent<Transform>()->GetPosition());
		}

	}


	void Player::OnPushLB() {
		if (m_PushPull) {
			auto elapsedtime = App::GetApp()->GetElapsedTime();
			auto ptrTransform = GetComponent<Transform>();
			auto pos = ptrTransform->GetPosition();
			auto obj = m_PushObj->GetComponent<Transform>();
			auto objpos = obj->GetPosition();
			auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			if (Cont.fThumbLX > 0.8f) {
				pos.x += elapsedtime;
				objpos.x += elapsedtime;
				GetComponent<Transform>()->SetPosition(pos);
				obj->SetPosition(objpos);
			}
			else if (Cont.fThumbLX < -0.8f) {
				pos.x -= elapsedtime;
				objpos.x -= elapsedtime;
				GetComponent<Transform>()->SetPosition(pos);
				obj->SetPosition(objpos);
			}
			if (Cont.fThumbLY > 0.8f) {
				pos.z += elapsedtime;
				objpos.z += elapsedtime;
				GetComponent<Transform>()->SetPosition(pos);
				obj->SetPosition(objpos);
			}
			else if (Cont.fThumbLY < -0.8f) {
				pos.z -= elapsedtime;
				objpos.z -= elapsedtime;
				GetComponent<Transform>()->SetPosition(pos);
				obj->SetPosition(objpos);
			}
		}

	}

	void Player::OnRemoveLB() {
		m_PushObj = nullptr;
		m_PushPull = false;
	}

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Obj) {
		auto ptrTransform = GetComponent<Transform>();
		if (Obj->FindTag(L"Deth")) {
			ptrTransform->SetPosition(0.0f, 0.5f, 0.0f);
			ptrTransform->SetRotation(0.0f, 0.0f, 0.0f);
			m_StopActionTimeJudge = true;
			m_PlayerState = PlayerState::Start;
		}
		if (Obj->FindTag(L"Goal")) {
			m_PlayerState = PlayerState::Clear;
		}
		if (Obj->FindTag(L"PushPullObj")) {
			m_PushPull = true;
			m_PushObj = Obj;
		}

		auto Ptr = dynamic_pointer_cast<GoalTest>(Obj);
		if (Ptr)
		{
			Ptr->SetGoal(true);
			Ptr->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0, 0, 0));
			//�S�[���G�t�F�N�g�Đ�
			auto Pos = Ptr->GetComponent<Transform>()->GetPosition();
			Pos.y += 0.5f;
			GetTypeStage<GameStage>()->Effectplay(L"GOAL_EFK",Pos);
			m_FireEfk->StopEffect();
		}
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj->FindTag(L"PossibleJump")) {
			m_Jumpjudge = true;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Obj) {
	}

	void Player::OnUpdate() {
		StateUpdate();
	}

	void Player::StateUpdate() {
		switch (m_PlayerState) 
		{
			case PlayerState::Start:
			{
				m_StopActionTimeJudge = true;
				StartBehavior();
				break;
			}
			case PlayerState::Excute:
			{
				m_StopActionTimeJudge = false;
				ExcuteBehavior();
				break;
			}
			case PlayerState::Clear:
			{
				m_StopActionTimeJudge = true;
				ClearBehavior();
				break;
			}
			default:
				break;
		}
	}

	void Player::StartBehavior()
	{
		//auto elapsedtime = App::GetApp()->GetElapsedTime();
		//if (m_StopActionTimeJudge) {
		//	m_StopActionTime -= elapsedtime;
		//}
		//if (m_StopActionTime <= 0.0f) {
		//	m_StopActionTimeJudge = false;
		//	m_PlayerState = PlayerState::Excute;
		//	m_StopActionTime = 5.0f;
		//}
	}

	void Player::ExcuteBehavior()
	{
		m_Handler.PushHandler(GetThis<Player>());
		
		Move();

		//�G�t�F�N�g�̈ړ�
		auto TransComp = GetComponent<Transform>();
		Vec3 EfkPoint = TransComp->GetPosition();
		EfkPoint.y += 1.0f;
		m_FireEfk->SetLocation(EfkPoint);
	}
	void Player::ClearBehavior()
	{
		//auto elapsedtime = App::GetApp()->GetElapsedTime();
		//if (m_StopActionTimeJudge) {
		//	m_StopActionTime -= elapsedtime;
		//}
		//if (m_StopActionTime <= 0.0f) {
		//	m_StopActionTime = 5.0f;
		//}
	}
}
//end basecross

