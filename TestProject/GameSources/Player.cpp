/*!
@file Player.cpp
@brief プレイヤーなど実体
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
		//タグをつける
		AddTag(L"Player");
		//影をつける（シャドウマップを描画する）
		auto shadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
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

		auto device = App::GetApp()->GetInputDevice(); // 入力装置を取得する
		auto pad = device.GetControlerVec()[0]; // 入力装置の中から０番目のパッドを取得する

		float delta = App::GetApp()->GetElapsedTime();
		auto transComp = GetComponent<Transform>();
		auto pos = transComp->GetPosition(); // 今現在の座標を取得する
		pos += 5.0f * Vec3(pad.fThumbLX, 0.0f, pad.fThumbLY) * delta;	// スティックの傾きに応じて最大で秒速5.0f移動させる
				// スティックが倒れていたらその向きに回転させる
		if (pad.fThumbLX != 0.0f && pad.fThumbLY != 0.0f) {
			// atan2f関数を用いて、スティックのXYをラジアン角に変換する
			transComp->SetRotation(0.0f, atan2f(-pad.fThumbLY, pad.fThumbLX), 0.0f); // X軸中心の回転, Y軸中心の回転, Z軸中心の回転をラジアン角で設定する
			transComp->SetPosition(pos);
			forward = Vec3(pad.fThumbLX, 0, pad.fThumbLY).normalize(); // 前方向を示すベクトルを算出する
		}

		if (pad.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			auto transComp = GetComponent<Transform>();
			auto pos = transComp->GetPosition();
			pos = Vec3(pos.x, pos.y += 5, pos.z);
			transComp->SetPosition(pos); // 座標を更新する
		}
	}

	Player::~Player() {}
}
//end basecross

