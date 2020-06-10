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
		m_StopActionTimeJudge(false),
		m_PushBoxActiv(false)
	{}



	void Player::OnCreate() {
		DefaultSettings();
		SetActions();
		auto ptrTransform = GetComponent<Transform>();

		AddComponent<Gravity>();

		m_PlayerState = PlayerState::Start;
		
		//���L�o�^
		//GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto ptrGameStage = GetTypeStage<GameStage>();
		auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(ptrGameStage->GetMainView()->GetCamera());
		ptrMyCamera->SetTargetObject(GetThis<Player>());


		AddTag(L"Player");
		//�΂��Đ�
		auto EfkPoint = m_pos;
		EfkPoint.y += 0.5f;
		m_FireEfk = ObjectFactory::Create<EfkPlay>(L"FIRE_EFK", EfkPoint);
		m_ClearSound = nullptr;
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
	//����֐�
	//����@����
	//�v���C���[���������߂̊֐�
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

	//L�{�^���������ݎ��̊֐�
	//����@����
	//�����𖞂�������ԂŃ{�^�����������Ƃŕ���������������������肷��֐�
	void Player::OnPushLB() {
		if (m_PushPull) {
			auto elapsedtime = App::GetApp()->GetElapsedTime();
			auto ptrTransform = GetComponent<Transform>();
			auto pos = ptrTransform->GetPosition();
			auto obj = m_PushObj->GetComponent<Transform>();
			auto objpos = obj->GetPosition();
			auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];
			m_PushBoxActiv = true;
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
	//L�{�^���������Ƃ��̊֐�
	//����@����
	//�{�^���𗣂����Ƃ��ɔ�������֐�
	void Player::OnRemoveLB() {
		PushPullRelese();
	}
	//�v�b�V���v�������֐�
	//����@����
	//���̉����o�������������������֐�
	void Player::PushPullRelese() {
		m_PushObj = nullptr;
		m_PushPull = false;
		m_PushBoxActiv = false;
	}
	//�ڐG������֐�
	//����@����
	//�I�u�W�F�N�g�̏Փˎ��Ɋe�����ɂ������������s���֐��i�S�[�����̏����A���̉������݈��������Ԃ̏����j
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Obj) {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrGetPos = ptrTransform->GetPosition();
		if (Obj->FindTag(L"Goal")) {
			m_PlayerState = PlayerState::Clear;
			auto Ptr = dynamic_pointer_cast<GoalTest>(Obj);
			if (Ptr)
			{
				Ptr->SetGoal(true);
				Ptr->GetComponent<PNTStaticDraw>()->SetEmissive(Col4(1.0f, 0, 0, 0));
				//�S�[���G�t�F�N�g�Đ�
				auto Pos = Ptr->GetComponent<Transform>()->GetPosition();
				Pos.y += 0.5f;
				GetTypeStage<GameStage>()->Effectplay(L"GOAL_EFK", Pos);
				m_FireEfk->StopEffect();
				if (!m_ClearSound) 
				{
					m_ClearSound = App::GetApp()->GetXAudio2Manager()->Start(L"FireStart_SD", 0, 1.0f);
				}
			}
		}
		if (Obj->FindTag(L"MoveFloor"))
		{
			ptrTransform->SetParent(Obj);
		}

		if (Obj->FindTag(L"PushPullObj")) {
			auto ptrPullBox = dynamic_pointer_cast<PushObj>(Obj);
			if (ptrPullBox) 
			{
				auto ptrBoxPos = ptrPullBox->GetCurrentPos();
				auto ptrBoxScal = ptrPullBox->GetComponent<Transform>()->GetScale();
				Vec3 BoxScale = ptrBoxScal / 2;
				if (ptrBoxPos.y + BoxScale.y > ptrGetPos.y) {
					m_PushPull = true;
					m_PushObj = Obj;

				}
			}
		}
	}
	//�ڐG���֐�
	//����@����
	//�Փ˂��Ă���ԂɊe�������s���֐��i�W�����v�ł��邩�̏����A��q�ɏ�鏈���j
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj->FindTag(L"PossibleJump") && !Obj->FindTag(L"Ladder")) {
			auto ptrPos = GetComponent<Transform>()->GetWorldPosition();
			auto ptrJFloorPos = Obj->GetComponent<Transform>()->GetWorldPosition();
			auto ptrJFloorScale = Obj->GetComponent<Transform>()->GetScale();
			m_JumpPos = ptrJFloorPos.y + (ptrJFloorScale.y / 2);
			if (ptrPos.y > m_JumpPos) {
				m_Jumpjudge = true;

			}
		}
		else if (Obj->FindTag(L"PossibleJump") && Obj->FindTag(L"Ladder")) {
			auto Elapsedtime = App::GetApp()->GetElapsedTime();
			auto ptrTransform = GetComponent<Transform>();
			auto ptrPos = ptrTransform->GetPosition();
			m_RisePos += 0.4f * Elapsedtime;
			if (m_RisePos > 0.4f) {
				m_RisePos = 0.4f;
			}
			ptrTransform->SetPosition(ptrPos.x, ptrPos.y + m_RisePos, ptrPos.z);
		}

	}	
	//�ڐG�����֐�
	//����@����
	//�ڐG���Ă���I�u�W�F�N�g���痣���֐��i��q���痣��鎞�j
	void Player::OnCollisionExit(shared_ptr<GameObject>& Obj) {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		m_RisePos = 0.0f;

		if (Obj->FindTag(L"MoveFloor"))
		{
			ptrTransform->ClearParent();
		}
	}


	void Player::StopPhysics() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetUpdateActive(false);
		auto ptrGrav = GetComponent<Gravity>();
		ptrGrav->SetGravityZero();
		
	}

	void Player::FireHitTest()
	{
		bool Dirty = false;
		auto TransComp = GetComponent<Transform>();
		Vec3 EfkPoint = TransComp->GetWorldPosition();
		EfkPoint.y += 1.0f;
		m_FireEfk->SetLocation(EfkPoint);

		Vec3 PlayerPos = TransComp->GetWorldPosition();
		Vec3 Botton = EfkPoint;
		Botton.y += 0.5f;
		Vec3 Top = EfkPoint;
		Top.y += 5.0f;
		m_FireCapsule = CAPSULE(0.5f, Botton,Top);
		
		 //-- �}�b�v�t�@�C�����X�V�����܂ŃR�����g�� --
		for (auto& v : GetStage()->GetGameObjectVec())
		{
			if (v->FindTag(L"FireIgnore"))
				continue;

			auto CollObb = v->GetComponent<CollisionObb>(false);
			if (CollObb)
			{
				auto Obb = CollObb->GetObb();
				Vec3 recvec;
				if (HitTest::CAPSULE_OBB(m_FireCapsule, Obb, recvec))
				{
					float VecVal = abs(recvec.y - m_FireCapsule.m_PointBottom.y);
					float Val = abs(m_FireCapsule.m_PointTop.y - m_FireCapsule.m_PointBottom.y);
					float Length = VecVal / Val;

					float s = Length;
					if (Length > 1.0f)
					{
						s = 1.0f;
					}
					else if (Length <= 0.0f)
					{
						s = 0.1f;
					}

					m_FireEfk->SetScale(Vec3(1.0f,s,1.0f));
					m_FireEfk->SetLocation(Vec3(EfkPoint.x, EfkPoint.y + s / 2.0f, EfkPoint.z));
					Dirty = true;
					return;
				}
			}
		}

		if (!Dirty)
		{
			m_FireEfk->SetScale(Vec3(1.0f));
		}
	}

	void Player::OnUpdate() {
		if (!GameManager::GetManager()->GetUpdateActive())
		{
			//m_FireEfk->SetPaused(true);
			return;
		}
		if (!m_ResetActive) {
			StateUpdate();
		}
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
			case PlayerState::Restart:
			{
				m_StopActionTimeJudge = true;
				RestartBehabior();
				break;
			}
			default:
				break;
		}
	}

	void Player::StartBehavior()
	{

	}

	void Player::ExcuteBehavior()
	{
		m_Handler.PushHandler(GetThis<Player>());
		
		Move();

		//�G�t�F�N�g�̈ړ�
		//m_FireEfk->SetPaused(false);
		FireHitTest();
	}
	void Player::ClearBehavior()
	{

	}

	void Player::RestartBehabior() {
		m_FireEfk->StopEffect();
		//�t�F�[�h�̊J�n�@�t�F�[�h���ĊJ�@�t�F�[�h�I����@�����Ȃ胊�Z�b�g���Ă΂�ā@�t�F�[�h�A�E�g�Ă΂�ā@���邭�Ȃ��ăt�F�[�h�������ē�����悤�ɂȂ�
		//�Q�[���X�e�[�W�̃}�l�[�W���[�Ńt�F�[�h�����߂�
		if (GameManager::GetManager()->GetStageReloadActive()) 
		{
			PostEvent(0.0f, GetThis<Player>(), L"Fade", L"ToGameStage", L"FadeOut");
		}
		else
		{
			PostEvent(0.0f, GetThis<Player>(), L"Fade", L"ReStart", L"FadeOut");
		}
		m_ResetActive = true;
	}

	void Player::OnEvent(const shared_ptr<Event>& Eve) {
		if (Eve->m_MsgStr == L"ExcuteActive") {
		}
		else if (Eve->m_MsgStr == L"ReStart") {
			ResetPositon();
			m_PlayerState = PlayerState::Excute;

		}
	}
}
//end basecross

