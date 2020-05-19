#include "stdafx.h"
#include "Project.h"


namespace basecross
{
Gate2::Gate2(const shared_ptr<Stage>& StagePtr,
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
Gate2::~Gate2() {}
void Gate2::OnCreate() {
	auto ptrTransform = GetComponent<Transform>();
	ptrTransform->SetScale(m_Scale);
	ptrTransform->SetRotation(m_Rotation);
	ptrTransform->SetPosition(m_Position);
	//�^�O������
	AddTag(L"Floor");
	//�e������i�V���h�E�}�b�v��`�悷��j
	auto shadowPtr = AddComponent<Shadowmap>();
	//�e�̌`�i���b�V���j��ݒ�
	shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
	auto ptrDraw = AddComponent<PNTStaticDraw>();
	ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
	ptrDraw->SetTextureResource(L"gate");
	ptrDraw->SetOwnShadowActive(true);

	PsBoxParam param(ptrTransform->GetWorldMatrix(), 0.0f, true,
		PsMotionType::MotionTypeFixed);
	auto PsPtr = AddComponent<RigidbodyBox>(param);
	PsPtr->SetAutoTransform(false);
	AddComponent<CollisionObb>();
}
void Gate2::GateOpen2()
{
	auto deltatime = App::GetApp()->GetElapsedTime();
	auto Pos = GetComponent<Transform>()->GetPosition();



	//if (Pos.x > 3)
	//{
	//	OpenSpeed = -1;
	//}
	//if (Pos.x < 1)
	//{
	//	OpenSpeed = +1;
	//}

	//y�����ɃQ�[�g���J��
	Pos.y += OpenSpeed*deltatime;
	GetComponent<Transform>()->SetPosition(Pos);
}
void Gate2::OnUpdate()
{
	if (gateopen2)
	{
		//�Q�[�g���J���t���O
		Gate2::GateOpen2();
	}
}

}