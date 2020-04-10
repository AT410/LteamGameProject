/*!
@file Player.h
@brief プレイヤーなど
@name 作成者：松崎洸樹（PlaerState enumクラス, Playerクラス）
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
		const int m_cntlNum;
		//プレイヤーの場所
		Vec3 m_Position;
		//プレイヤーの角度
		Vec3 m_Rotation;
		//プレイヤーの大きさ
		Vec3 m_Scale;
		//プレイヤーの進行方向の向き
		Vec3 m_PlayerAngle;
		Vec3 GetMoveVector() const;
		//コントローラー操作取得
		Controller m_cntl;
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
		InputHandler<Player> m_inputHandler;
	public:
		Player(const shared_ptr<Stage>& Stageptr);
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
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj)override;
	};

}
//end basecross

