/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	Player::Player(const shared_ptr<Stage>& StagePtr,
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
	void Player::OnCreate() {
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
	void Player::OnUpdate()
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

	Player::~Player() {}
}
//end basecross

