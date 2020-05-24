/*!
@breif	Gimmick関連
@name	作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>スイッチオブジェクト<breif/>
	///<name>作成者：阿部達哉<name/>
	class SwitchObj : public ObjectBase
	{
	public:
		SwitchObj(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey,const bool IsKeep = false)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_IsKeep(IsKeep), m_Active(false)
		{
		}

		SwitchObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		void OnCreate() override;
		void OnUpdate()override
		{
			auto transComp = GetComponent<Transform>();
			transComp->SetPosition(m_pos);
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void OnCollisionExit(shared_ptr<GameObject>& Other)	override;

		//ゲッター
		bool GetSwitchActiveI()const { return m_Active; }
	private:
		bool m_Active;//ON・OFF判定
		//押し続ける必要があるか。
		bool m_IsKeep;

		//イベント設定
		wstring m_RecipientKey;//<-受信先設定キー
		wstring m_EventMsg; //<-イベントメッセージ

		//エフェクト
		shared_ptr<EfkPlay> m_ActiveEfk;
	};

	//----------------------------------------------------------------------------
	//導火線
	//----------------------------------------------------------------------------
	class FireLine : public ObjectBase
	{
	public:
		//-- 構築 --
		FireLine(const shared_ptr<Stage>& StagePtr)
			:ObjectBase(StagePtr)
		{
		}

		FireLine(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);


		//-- 破棄 --
		virtual ~FireLine() {}

		//-- 初期化 --
		void OnCreate()override;

		//-- 更新処理 --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>&event);

	private:
		float m_Time = 0;
		bool m_Active = false;

		//イベント設定
		wstring m_RecipientKey;//<-受信先設定キー
		wstring m_EventMsg; //<-イベントメッセージ
	};

	///<breif>熱を伝える棒<breif/>
	///<name> 作成者：伊藤祥吾<name/>
	class HeatStick : public ObjectBase
	{
	private:
		AABB m_HeatArea;

		shared_ptr<Player> m_player;
		shared_ptr<ObjectBase> m_himo;

		//ディレイカウント
		float m_count;
	public:
		HeatStick(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_count(0)
		{
		}
		HeatStick(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;
	};

	//おもり
	class Omori : public ObjectBase
	{
	public:
		Omori(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Omori(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	//動く床
	class MoveFloor : public ObjectBase
	{
	private:
		float m_Time;
		bool m_Active; //flg
	public:
		MoveFloor(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		MoveFloor(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}


		void OnCreate() override;
		void OnUpdate() override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};
	//-----------------------------------------------------------------------------
	//扉のオブジェクト
	//-----------------------------------------------------------------------------
	class Door :public ObjectBase
	{
	public:
		Door(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr){}

		// -- Xmlファイル読込用 --
		Door(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~Door(){}

		// -- 初期化 --
		void OnCreate()override;

		// -- 更新処理 --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>& event)override;

		// -- セッター --
		void SetOpenActive(bool Active) { m_OpenActive = Active; }

	private:
		// -- 開門処理 --
		void OpenProcess();
		// -- 閉門処理 --
		void CloseProcess();

		bool MoveBehavior(Vec3 Start,Vec3 End, const float TotalTime);

		float m_TotalTime;
		bool m_OpenActive;
		bool m_MoveEnd;
	};

	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	class Fountain :public ObjectBase
	{
	private:
		bool m_Active; //flg
		float m_Time = 0;

		AABB m_FountainArea; //常に有効
		AABB m_FountainArea2; //水がActiveの時だけに有効

		shared_ptr<Player> m_TestPlayer;

		shared_ptr<EfkPlay> m_WaterEfk;

	public:
		Fountain(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Fountain(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};


	//-----------------------------------------------------------------------------
	//滝クラス
	//-----------------------------------------------------------------------------
	class Waterfall :public ObjectBase
	{
	public:
		Waterfall(const shared_ptr<Stage>&StagePtr,const Vec3& Start,const Vec3& End,const float Width,const float Speed)
			:ObjectBase(StagePtr),m_StartPoint(Start),m_EndPoint(End),m_Width(Width),m_FallSpeed(Speed){}

		Waterfall(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		Vec3 m_StartPoint;
		Vec3 m_EndPoint;

		float m_Width;
		float m_FallSpeed;

		float m_TotalTime = 0;

		shared_ptr<EfkPlay> m_EfkPlay;
	};

	//ウォータージェットクラス
	//＠製作者　松崎洸樹
	class WaterJet : public ObjectBase {
		Vec3 m_Pos;
		Vec3 m_StartPos;
		bool m_WaterJetmode; //水噴射のON/OFFスイッチ
		bool m_WaterJetDirToUp; //水噴射の向き　trueが上向き falseが下向き
		AABB m_WaterJetAABB;
		float m_JudmentTime;
		float m_SizeAABBX;
		float m_SizeAABBY;
		float m_SizeAABBZ;

		shared_ptr<EfkPlay> m_efk;
		//イベント設定
		wstring m_RecipientKey;//<-受信先設定キー
		wstring m_EventMsg; //<-イベントメッセージ


	public:
		WaterJet(const shared_ptr<Stage>& Stageptr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			: ObjectBase(Stageptr, Position, Rotation, Scale, TexKey, MeshKey),	m_WaterJetmode(false),m_WaterJetDirToUp(false),
			m_SizeAABBX(2.0f),m_SizeAABBY(3.0f),m_SizeAABBZ(2.0f)
		{}
		WaterJet(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterJet() {}
		void SetSizeAABBX(const float sizeX);
		void SetSizeAABBY(const float sizeY);
		void SetSizeAABBZ(const float sizeZ);
		void WaterJetJudgment();
		void StartJudgment();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	class WaterDrop : public ObjectBase {
		float m_Speed;
		float m_FallSpeed;
		float m_time;
		float m_SpeedAdd;
		float m_WaterAABBX;
		float m_WaterAABBY;
		float m_WaterAABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		AABB m_WaterDropAABB;
	public:
		WaterDrop(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(0.0f),m_FallSpeed(0.0f),m_time(3.0f),m_SpeedAdd(0.4f)
		{}
		WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterDrop() {}
		void Drop();
		void WaterDropJudgement();
		virtual void OnCreate();
		virtual void OnUpdate();

		void OnCollisionEnter(shared_ptr<GameObject>&Obj)override;

	};

	//ウォーターレベルクラス
//＠制作者　松崎洸樹
	class WaterLV : public ObjectBase {
		float m_Waterheight;
		float m_WaterTime;
		float m_AABBX;
		float m_AABBY;
		float m_AABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		bool m_WaterLVMode;
		Vec3 m_WaterOldPos;
		Vec3 m_WaterCurrentPos;
		AABB m_WaterLVAABB;
		AABB m_PlayerAABB;
	public:
		WaterLV(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey) 
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey),m_WaterLVMode(false),m_AABBX(1.0f),m_AABBY(1.0f),m_AABBZ(1.0f) {}
		WaterLV(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterLV() {}
		void SetWaterLVMode(const bool LVMode);
		void WaterLVChange();
		void WaterLVJudgement();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void OnEvent(const shared_ptr<Event>&event);
	};


	class UpDownBox : public ObjectBase {
		float m_Speed;
		float m_totaltime;
		float m_parenttime;
		AABB m_BoxAABB;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		bool m_ParentJudge;
		bool FloatMove();
	public:
		UpDownBox(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(1.0f), m_OldPos(0.0f), m_parenttime(2.0f){}
		UpDownBox(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~UpDownBox() {}
		void BoxJudgment();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

}