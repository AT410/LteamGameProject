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
		bool m_Active;				//<-ON・OFF判定

		bool m_IsKeep;				//<-押し続ける必要があるか。

		//イベント設定
		wstring m_RecipientKey;		//<-受信先設定キー
		wstring m_EventMsg;			//<-イベントメッセージ

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
		float FireLineBehaviorPos(float Key, float Pos)
		{
			return Pos += Key * 0.025f;
		}

		float FireLineBehaviorScale(float Scale)
		{
			return Scale += -0.05f;
		}

		enum FireLineConfigu
		{
			None = 0,
			LeftToRight,
			RightToLeft,
			UpToDown,
			DownToUp,
			FrontToBack,
			BackToFront
		}m_Configu = FireLine::None;

		float m_Time = 0;
		bool m_Active = false;

		shared_ptr<EfkPlay> m_Fire;

		//イベント設定
		wstring m_RecipientKey;	//<-受信先設定キー
		wstring m_EventMsg;		//<-イベントメッセージ
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
		void OnEvent(const shared_ptr<Event>& event);

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};

	//----------------------------------------------------------------------------
	//スロープオブジェクト
	//----------------------------------------------------------------------------
	class Slope : public ObjectBase
	{
	public:
		//-- 構築 --
		Slope(const shared_ptr<Stage>& StagePtr) 
			:ObjectBase(StagePtr)
		{
		}

		// -- Xmlファイル読込用 --
		Slope(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);


		//-- 破棄 --
		virtual ~Slope() {}

		//-- 初期化 --
		void OnCreate()override;

		//-- 更新処理 --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>& event)override;

	private:

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

		virtual ~Waterfall()
		{
			App::GetApp()->GetXAudio2Manager()->Stop(m_WaterSound);
		}

		void OnCreate()override;

		void OnUpdate()override;

		void StartSound()
		{
			if (!m_SoundActive) {
				m_WaterSound = App::GetApp()->GetXAudio2Manager()->Start(L"WaterFall_SD", XAUDIO2_LOOP_INFINITE, 0.25f);
				m_SoundActive = true;
			}
		}

		void StopSound()
		{
			App::GetApp()->GetXAudio2Manager()->Stop(m_WaterSound);
			m_SoundActive = false;
		}

		void UpdateEndPoint(const Vec3& EndPoint)
		{
			float width = m_Width / 2.0f;

			m_vertices =
			{
				{Vec3(EndPoint.x - width,EndPoint.y,0.0f), Vec3(1,0,1),Vec2(0,0)},
				{Vec3(EndPoint.x + width,EndPoint.y,0.0f), Vec3(1,0,1),Vec2(1,0)},
				{Vec3(m_StartPoint.x - width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(0,1)},
				{Vec3(m_StartPoint.x + width,m_StartPoint.y,0.0f), Vec3(1,0,1),Vec2(1,1)},
			};
		}

	private:
		Vec3 m_StartPoint;
		Vec3 m_EndPoint;

		float m_Width;
		float m_FallSpeed;

		float m_TotalTime = 0;

		bool m_SoundActive;

		shared_ptr<EfkPlay> m_EfkPlay;

		shared_ptr<SoundItem> m_WaterSound;

		vector<VertexPositionNormalTexture> m_vertices;
	};

	//ウォータージェットクラス
	//＠製作者　松崎洸樹
	class WaterJet : public ObjectBase {
		//初期位置
		Vec3 m_StartPos;
		//水噴射の水スイッチ
		bool m_WaterJetmode; //水噴射の水ON/OFFスイッチ
		//水噴射の向き
		bool m_WaterJetDirToUp; //水噴射の向き　trueが上向き falseが下向き
		//水噴射の水部分のAABB用変数
		AABB m_WaterJetAABB;
		//噴射前段階での
		float m_JudmentTime;
		//水噴射時間間隔
		float m_IntervalTime;
		//水噴射作動間隔フラグ
		bool m_IntervalFlag;
		//
		float m_FlashTime;
		//AABB・X軸
		float m_SizeAABBX;
		//AABB・Y軸
		float m_SizeAABBY;
		//AABB・Z軸
		float m_SizeAABBZ;
		//エフェクト用
		shared_ptr<EfkPlay> m_efk;
		//水本体
		shared_ptr<Waterfall> m_WaterFall;
		//衝突する床
		weak_ptr<GameObject> m_Floor;
	public:
		WaterJet(const shared_ptr<Stage>& Stageptr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			: ObjectBase(Stageptr, Position, Rotation, Scale, TexKey, MeshKey),	m_WaterJetmode(false),m_WaterJetDirToUp(false),
			m_SizeAABBX(2.0f),m_SizeAABBY(30.0f),m_SizeAABBZ(2.0f)
		{}
		WaterJet(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterJet() {}
		void SetSizeAABBX(const float sizeX);
		void SetSizeAABBY(const float sizeY);
		void SetSizeAABBZ(const float sizeZ);
		void WaterJetJudgment();
		void StartJudgment();

		// -- 真下の地面を取得する --
		void GetUnderFloor();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	class WaterDrop : public ObjectBase {

		float m_Time;
		//元の位置から水滴が出るまでの時間
		float m_Cooltime;
		//クールタイムの最大値
		float m_CooltimeMax;
		//初期位置
		Vec3 m_OldPos;
		//現在位置
		Vec3 m_CurrentPos;
	public:
		WaterDrop(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{}
		WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterDrop() {}
		void CoolTime();
		virtual void OnCreate();
		virtual void OnUpdate();

		void OnCollisionEnter(shared_ptr<GameObject>&Obj)override;

	};

	//----------------------------------------------------------------------------
	//水位オブジェクト：変更
	//----------------------------------------------------------------------------
	class WaterLV2 : public ObjectBase
	{
	public:
		//-- 構築 --
		WaterLV2(const shared_ptr<Stage>& StagePtr) 
			:ObjectBase(StagePtr)
		{
		}

		// -- Xml構築 --
		WaterLV2(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);

		//-- 破棄 --
		virtual ~WaterLV2() {}

		//-- 初期化 --
		void OnCreate()override;

		//-- 更新処理 --
		void OnUpdate()override;

		// -- イベント処理 --
		void OnEvent(const shared_ptr<Event>&event);

		// -- 衝突応答 --
		void OnCollisionEnter(shared_ptr<GameObject>&Obj)override;
		void OnCollisionExcute(shared_ptr<GameObject>&Obj)override;
		void OnCollisionExit(shared_ptr<GameObject>&Obj)override;
	private:
		// -- 水位変更 --
		void ChangeLevel();

		void HitResponseAABB();

		bool LVBhavior(const float ScaleVal);

		float m_IntervalTime;

		bool m_MainActive;

		bool m_IntervalActive;

		bool m_End;
		bool m_ContFlag;//true =上昇 false = 下降
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
		//トータルタイムのための時間
		float m_Totaltime;
		//プレイヤーとの疑似的親子化の時間
		float m_Parenttime;
		//初期位置
		Vec3 m_OldPos;
		//現在位置
		Vec3 m_CurrentPos;
		//疑似的親子化の判定
		bool m_ParentJudge;
		//動く床の機能
		bool FloatMove();
	public:
		UpDownBox(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey){}
		UpDownBox(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~UpDownBox() {}
		void BoxJudgment();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

	class PushObj : public ObjectBase {
		//オブジェクトが動けるか動けないか
		bool m_Boxmode;
		//オブジェクトの現在の位置
		Vec3 m_CurrentPos;
		//オブジェクトの停止位置
		Vec3 m_StopPos;
		//オブジェクトの以前の位置
		Vec3 m_PastPos;
	public:
		PushObj(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey) {}
		PushObj(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~PushObj() {}
		void BoxState();
		Vec3 GetCurrentPos() { return m_CurrentPos; }
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Obj)override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


	//----------------------------------------------------------------------------
	//ゴールクラス
	//----------------------------------------------------------------------------
	class Goal :public ObjectBase
	{
	public:
		Goal(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~Goal() {}

		void OnCreate()	override;

		void OnUpdate()override;

		void SetGoal(bool IsGoal) { m_IsGoal = IsGoal; }

	private:
		bool m_IsGoal;
		float m_count;

		shared_ptr<SoundItem> m_sound;
	};
}