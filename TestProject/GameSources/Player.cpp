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
		m_cntlNum(0),
		m_Speed(3.0f),
		m_Jumpforce(4.0f),
		m_StopActionTime(5.0f),
		m_Jumpjudge(false),
		m_StopActionTimeJudge(false)
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
		m_PlayerState = PlayerState::Start;
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
		if (m_PlayerAngle.length() > 0.0f && !m_PushPull) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += m_PlayerAngle * elapsedtime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos);
		}
		if (m_PlayerAngle.length() > 0.0f && !m_PushPull) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(m_PlayerAngle, 1.0f);
		}
	}

	void Player::StartState() {
		auto elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_StopActionTimeJudge) {
			m_StopActionTime -= elapsedtime;
		}
		if (m_StopActionTime <= 0.0f) {
			m_StopActionTimeJudge = false;
			m_PlayerState = PlayerState::Excute;
			m_StopActionTime = 5.0f;
		}

	}

	void Player::ClearState() {
		auto elapsedtime = App::GetApp()->GetElapsedTime();
		if (m_StopActionTimeJudge) {
			m_StopActionTime -= elapsedtime;
		}
		if (m_StopActionTime <= 0.0f) {
			m_StopActionTime = 5.0f;
		}
	}

	void Player::OnPushA() {
		if (m_Jumpjudge && !m_StopActionTimeJudge) {
			auto grav = GetComponent<Gravity>();
			grav->StartJump(Vec3(0.0f, m_Jumpforce, 0.0f));
			m_Jumpjudge = false;
		}

	}


	void Player::OnPushLB() {
		auto elapsedtime = App::GetApp()->GetElapsedTime();
		m_PlayerAngle = GetMoveVector();
		auto ptrTransform = GetComponent<Transform>();
		auto pos = ptrTransform->GetPosition();
		auto playerRoll = ptrTransform->GetRotation();
		auto utilePtr = GetBehavior<UtilBehavior>();
		if (m_PushPull) {
			auto obj = m_PushObj->GetComponent<Transform>();
			auto objpos = obj->GetPosition();
			auto objscale = obj->GetScale();
			auto HalfObjScale = objscale / 2;

			if (pos.x < objpos.x&& pos.z < objpos.z + HalfObjScale.z && pos.z > objpos.z - HalfObjScale.z) {
				utilePtr->RotToHead(Vec3(0.0, 90.0f, 0.0f), 1.0f);
				if (m_cntl.LX > 0.8f) {
					pos.x += elapsedtime;
					objpos.x += elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
				else if (m_cntl.LX < -0.8f) {
					pos.x -= elapsedtime;
					objpos.x -= elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
			}
			else if (pos.x > objpos.x&& pos.z < objpos.z + HalfObjScale.z && pos.z > objpos.z - HalfObjScale.z) {
				utilePtr->RotToHead(Vec3(0.0, -90.0f, 0.0f), 1.0f);
				if (m_cntl.LX > 0.8f) {
					pos.x += elapsedtime;
					objpos.x += elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
				else if (m_cntl.LX < -0.8f) {
					pos.x -= elapsedtime;
					objpos.x -= elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
			}
			else if (pos.z > objpos.z&& pos.x < objpos.x + HalfObjScale.x && pos.x > objpos.x - HalfObjScale.x) {
				utilePtr->RotToHead(Vec3(0.0, 180.0f, 0.0f), 1.0f);
				if (m_cntl.LY > 0.8f) {
					pos.z += elapsedtime;
					objpos.z += elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
				else if (m_cntl.LY < -0.8f) {
					pos.z -= elapsedtime;
					objpos.z -= elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
			}
			else if (pos.z < objpos.z && pos.x < objpos.x + HalfObjScale.x && pos.x > objpos.x - HalfObjScale.x) {
				utilePtr->RotToHead(Vec3(0.0, 0.0f, 0.0f), 1.0f);
				if (m_cntl.LY > 0.8f) {
					pos.z += elapsedtime;
					objpos.z += elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
				else if (m_cntl.LY < -0.8f) {
					pos.z -= elapsedtime;
					objpos.z -= elapsedtime;
					GetComponent<Transform>()->SetPosition(pos);
					obj->SetPosition(objpos);
				}
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
	}

	void Player::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj) {
			m_Jumpjudge = true;
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Obj) {
	}

	void Player::OnUpdate() {
		m_inputHandler.PushHandle(m_cntlNum, m_cntl);
		m_Handler.PushHandler(GetThis<Player>());
		State();
	}

	void Player::State() {
		switch (m_PlayerState){
		case PlayerState::Start: 
		{
			m_StopActionTimeJudge = true;
			StartState();
		}
			break;
		case PlayerState::Excute:
		{
			Move();
		}
		break;
		case PlayerState::Clear:
		{
			ClearState();
		}
		break;
		}
	}


Playerhurukawa::Playerhurukawa(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	void Playerhurukawa::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		PlayerSpeed = 3;
		//�^�O������
		AddTag(L"Player");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		/*ptrDraw->SetTextureResource(L"SKY_TX");*/
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

		PsBoxParam param(ptrTransform->GetWorldMatrix(), 1.0f, false,
			PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
		PsPtr->SetAutoTransform(false);
		AddComponent<Gravity>();
		AddComponent<CollisionObb>();

	}
	void Playerhurukawa::OnUpdate()
	{

		auto device = App::GetApp()->GetInputDevice(); // ���͑��u���擾����
		auto pad = device.GetControlerVec()[0]; // ���͑��u�̒�����O�Ԗڂ̃p�b�h���擾����

		float delta = App::GetApp()->GetElapsedTime();
		auto transComp = GetComponent<Transform>();
		auto pos = transComp->GetPosition(); // �����݂̍��W���擾����
		pos += 5.0f * Vec3(pad.fThumbLX, 0.0f, pad.fThumbLY) * delta;	// �X�e�B�b�N�̌X���ɉ����čő�ŕb��5.0f�ړ�������
				// �X�e�B�b�N���|��Ă����炻�̌����ɉ�]������
		if (pad.fThumbLX != 0.0f && pad.fThumbLY != 0.0f) {
			// atan2f�֐���p���āA�X�e�B�b�N��XY�����W�A���p�ɕϊ�����
			transComp->SetRotation(0.0f, atan2f(-pad.fThumbLY, pad.fThumbLX), 0.0f); // X�����S�̉�], Y�����S�̉�], Z�����S�̉�]�����W�A���p�Őݒ肷��
			transComp->SetPosition(pos);
			forward = Vec3(pad.fThumbLX, 0, pad.fThumbLY).normalize(); // �O�����������x�N�g�����Z�o����
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			auto transComp = GetComponent<Transform>();
			auto pos = transComp->GetPosition();
			pos = Vec3(pos.x, pos.y += 5, pos.z);
			transComp->SetPosition(pos); // ���W���X�V����
		}
	}

	Playerhurukawa::~Playerhurukawa() {}
}
//end basecross

