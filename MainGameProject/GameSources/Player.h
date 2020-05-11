/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//---------------------------------------------------
	//プレイヤーステート
	//@作成者：松崎洸樹
	//---------------------------------------------------
	enum class PlayerState {
		Start,
		Excute,
		Clear,
	};
	//---------------------------------------------------
	//プレイヤークラス
	//@作成者：松崎洸樹
	//---------------------------------------------------
	class Player : public ObjectBase, public PawnBase<Player> {
	private:
		//プレイヤーの進行方向の向き
		Vec3 m_PlayerAngle;
		//プレイヤーの速さ
		float m_Speed;
		//プレイヤーのジャンプの強さ
		float m_Jumpforce;
		//プレイヤーの動作停止時間
		float m_StopActionTime;
		//プレイヤーとオブジェクトの距離
		float m_distance;
		//プレイヤーのジャンプ判断
		bool m_Jumpjudge;
		//プレイヤーの動作停止判断
		bool m_StopActionTimeJudge;
		//押し出し引き判断
		bool m_PushPull;
		//プレイヤーステート取得変数
		PlayerState m_PlayerState;

		shared_ptr<GameObject> m_PushObj = nullptr;
		//入力ハンドラー
		InputHandler<Player> m_Handler;
		Vec3 GetMoveVector() const;

	public:
		Player(const shared_ptr<Stage>& Stageptr);

		Player(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false)

		{
		}

		Player(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false)
		{}

		virtual ~Player() {}
		//プレイヤー移動関数
		void Move();
		//ObjectBase継承->Aボタン入力ジャンプ関数
		void OnPushA() override;
		//objectBase継承->LBボタン入力物引っ張る関数
		void OnPushLB() override;
		void OnRemoveLB() override;
		//プレイヤーステート変化関数
		void State();
		//プレイヤースタートステート
		void StartState();
		//プレイヤーゴールステート	
		void ClearState();
		void OnCreate() override;
		void OnUpdate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExit(shared_ptr<GameObject>& Obj)override;
	
		void ResetPositon()
		{
			GetComponent<Transform>()->SetPosition(m_pos);
		}
	};


}
//end basecross

