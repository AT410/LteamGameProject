
#include "stdafx.h"
#include "Project.h"

namespace basecross {


	Button22::Button22(const shared_ptr<Stage>& StagePtr,
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
	void Button22::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//�^�O������
		AddTag(L"Button");
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
		AddComponent<CollisionObb>();
	}

	void Button22::OnUpdate()
	{
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
	}
	Button22::~Button22() {}
	void Button22::OnCollisionEnter(shared_ptr<GameObject>&other)
	{
		auto player = dynamic_pointer_cast<Playerhurukawa>(other);
		if (player)
		{
			//�������gate�̃t���O�������flg��true�̎��̓Q�[�g���J���悤�ɂ���
			GetStage()->GetSharedGameObject<Gate>(L"gate")->setopenflg(true);
			GetStage()->GetSharedGameObject<Gate2>(L"gate2")->setopenflg2(true);
		}
	}
}