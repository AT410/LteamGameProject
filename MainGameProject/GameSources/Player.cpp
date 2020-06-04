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
		m_StopActionTimeJudge(false),
		m_PushBoxActiv(false)
	{}



	void Player::OnCreate() {
		DefaultSettings();
		SetActions();
		auto ptrTransform = GetComponent<Transform>();

		AddComponent<CollisionObb>();
		AddComponent<Gravity>();

		auto Shadowptr = AddComponent<Shadowmap>();
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		m_PlayerState = PlayerState::Start;
		
		//共有登録
		//GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto ptrGameStage = GetTypeStage<GameStage>();
		auto ptrMyCamera = dynamic_pointer_cast<MyCamera>(ptrGameStage->GetMainView()->GetCamera());
		ptrMyCamera->SetTargetObject(GetThis<Player>());


		//火を再生
		auto EfkPoint = m_pos;
		EfkPoint.y += 0.5f;
		m_FireEfk = ObjectFactory::Create<EfkPlay>(L"FIRE_EFK", EfkPoint);

		m_ClearSound = nullptr;
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
	//動作関数
	//松崎　洸樹
	//プレイヤーが動くための関数
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

	//Lボタン押し込み時の関数
	//松崎　洸樹
	//条件を満たした状態でボタンを押すことで物を押したり引っ張ったりする関数
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
	//Lボタン離したときの関数
	//松崎　洸樹
	//ボタンを離したときに発動する関数
	void Player::OnRemoveLB() {
		PushPullRelese();
	}
	//プッシュプル解除関数
	//松崎　洸樹
	//物の押し出し引っ張りを解除する関数
	void Player::PushPullRelese() {
		m_PushObj = nullptr;
		m_PushPull = false;
		m_PushBoxActiv = false;
	}
	//接触時判定関数
	//松崎　洸樹
	//オブジェクトの衝突時に各条件にあった処理を行う関数（ゴール時の処理、物の押し込み引っ張り状態の処理）
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
				//ゴールエフェクト再生
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
		if (Obj->FindTag(L"PushPullObj")) {
			auto ptrPullBox = dynamic_pointer_cast<PushObj>(Obj);
			auto ptrBoxPos = ptrPullBox->GetCurrentPos();
			if (ptrBoxPos.y > ptrGetPos.y) {
				m_PushPull = true;
				m_PushObj = Obj;
			}
		}
	}
	//接触中関数
	//松崎　洸樹
	//衝突している間に各処理を行う関数（ジャンプできるかの処理、梯子に上る処理）
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Obj) {
		if (Obj->FindTag(L"PossibleJump") && !Obj->FindTag(L"Ladder")) {
			auto ptrPos = GetComponent<Transform>()->GetPosition();
			auto ptrJFloorPos = Obj->GetComponent<Transform>()->GetPosition();
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
	//接触解除関数
	//松崎　洸樹
	//接触しているオブジェクトから離れる関数（梯子から離れる時）
	void Player::OnCollisionExit(shared_ptr<GameObject>& Obj) {
		auto ptrTransform = GetComponent<Transform>();
		auto ptrPos = ptrTransform->GetPosition();
		m_RisePos = 0.0f;
	}

	void Player::OnUpdate() {
		if (!GameManager::GetManager()->GetUpdateActive())
			return;
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

	}
}
//end basecross

