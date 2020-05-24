/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//---------------------------------------------------
//プレイヤークラスの実体
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
		
		//共有登録
		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto ptrGameStage = GetTypeStage<GameStage>();
		auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(ptrGameStage->GetMainView()->GetCamera());
		ptrMyCamera->SetTargetObject(GetThis<Player>());

		for (auto tag : m_tag)
		{
			AddTag(tag);
		}

		//火を再生
		auto EfkPoint = m_pos;
		EfkPoint.y += 0.5f;
		m_FireEfk = ObjectFactory::Create<EfkPlay>(L"FIRE_EFK", EfkPoint);

	}

	//進行ベクトルへの下方関数
	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);

		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		float moveX = Cont.fThumbLX;
		float moveZ = Cont.fThumbLY;

		float moveLength = 0;   //動いた時のスピード
		auto ptrTransform = GetComponent<Transform>();
		auto ptrCamera = OnGetDrawCamera();
		//進行方向の向きを計算
		auto front = ptrTransform->GetWorldPosition() - ptrCamera->GetEye();
		front.y = 0;
		front.normalize();
		//進行方向向きの角度を算出
		float frontAngle = atan2(front.z, front.x);
		//コントローラの向き計算

		Vec2 moveVec(moveX, moveZ);
		float moveSize = moveVec.length();
		//コントローラーの向きから角度を計算
		float cntlAngle = atan2(-moveX, moveZ);
		//トータルの角度を算出
		float totalAngle = frontAngle + cntlAngle;
		//角度からベクトルを作成
		angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
		//正規化する
		angle.normalize();
		//移動サイズを設定
		angle *= moveSize;
		//Y軸は変化させない
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
			//ゴールエフェクト再生
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

		//エフェクトの移動
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

