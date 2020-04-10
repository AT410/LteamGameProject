/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"
#include "Player.h"

namespace basecross{

	//---------------------------------------------------
	//プレイヤークラスの実体
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


	//進行ベクトルへの下方関数
	Vec3 Player::GetMoveVector() const{
		Vec3 angle(0, 0, 0);
		float moveX = m_cntl.LX;
		float moveZ = m_cntl.LY;
		
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

	void Player::PushPull() {
	}

	void Player::OnPushLB() {
		auto elapsedtime = App::GetApp()->GetElapsedTime();
		m_PlayerAngle = GetMoveVector();
		auto ptrTransform = GetComponent<Transform>();
		auto pos = ptrTransform->GetPosition();
		auto playerRoll = ptrTransform->GetRotation();
		auto ptrCamera = OnGetDrawCamera();
		//進行方向の向きを計算
		auto front = ptrTransform->GetWorldPosition() - ptrCamera->GetEye();
		front.y = 0;
		front.normalize();
		//進行方向向きの角度を算出
		float frontAngle = atan2(front.z, front.x);
		auto angle = Vec3(cos(frontAngle), 0.0f, sin(frontAngle));
		angle.normalize();
		if (m_PushPull) {
			//ptrTransform->SetRotation(0.0f, 50.0f, 0.0f);
			auto obj = m_PushObj->GetComponent<Transform>();
			auto objpos = obj->GetPosition();
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
		case PlayerState::PullPush:
		{
			PushPull();
		}
		}
	}


}
//end basecross

