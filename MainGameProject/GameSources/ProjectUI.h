/*!
@breif	プロジェクト定義のUI
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class StageType;

	//----------------------------------------------------------------------------
	//UI基底クラス
	//----------------------------------------------------------------------------
	class UIBase :public GameObject
	{
	public:
		UIBase(const shared_ptr<Stage>&StagePtr);
		UIBase(const shared_ptr<Stage>&StagePtr,IXMLDOMNodePtr pNode);

		virtual ~UIBase();

		void OnCreate()override = 0;

		virtual void OnUpdate()override {}

		Vec3 GetStartPos()const;
		wstring GetTexKey()const;
		float GetUIWidth()const;
		float GetUIHeight()const;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//----------------------------------------------------------------------------
//通常UI;(常時表示)
//----------------------------------------------------------------------------
	class NormalUI :public UIBase
	{
	public:
		NormalUI(const shared_ptr<Stage>&StagePtr)
			:UIBase(StagePtr)
		{}

		NormalUI(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:UIBase(StagePtr, pNode)
		{}

		virtual ~NormalUI() {}

		void OnCreate()override;

	};

	//----------------------------------------------------------------------------
	//点滅UI	
	//----------------------------------------------------------------------------
	class FlashingUI :public UIBase
	{
	public:
		FlashingUI(const shared_ptr<Stage>&StagePtr
			,const wstring& Up, const wstring& Down, const wstring& Right, const wstring& Left)
			:UIBase(StagePtr),m_FlashingSpeed(0.5f),m_ActiveFlashing(true),
			m_Up(Up), m_Down(Down), m_Right(Right), m_Left(Left),m_MyKey(L"1"),m_EventStr(L"ToTitleStage") {}
		FlashingUI(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~FlashingUI(){}

		void OnCreate()override;

		void OnUpdate()override;

		void SetFlashingSpeed(const float Spped) { m_FlashingSpeed = Spped > 0 ? Spped : 0.5f; }
		void ChangeActive(const bool bActive) { m_ActiveFlashing = bActive; }

		void StartEvent();

		wstring GetUpStr() { return m_Up; }
		wstring GetDownStr() { return m_Down; }
		wstring GetRightStr() { return m_Right; }
		wstring GetLeftStr() { return m_Left; }

	private:
		wstring m_Up, m_Down, m_Right, m_Left,m_MyKey,m_EventStr;

		int m_StageNum,m_AreaNum;

		bool m_ActiveFlashing;
		float m_FlashingSpeed;
		float m_TotalTime;
	};

	class UIController :public GameObject,public PawnBase<UIController>
	{
	public:
		UIController(const shared_ptr<Stage>&StagePtr,const StageType Type,const bool DefalutActive = true)
			:GameObject(StagePtr), PawnBase(), m_UIMap(), m_Type(Type), m_Active(DefalutActive), m_IsSent(false)
		{

		}

		virtual ~UIController(){}

		void OnCreate()override;

		void OnUpdate()override;

		void ChangeActiveUI(const wstring& Key);

		void AddPawnUI(const wstring& UIkey, shared_ptr<FlashingUI>& UIPtr) { m_UIMap[UIkey] = UIPtr; }

		void SetCurrntUI();

		//入力ハンドラ
		void OnPushA()override;

		void OnPushB()override;

	private:
		// -- UI操作 --
		void OperationUI(const CONTROLER_STATE Cont);

		// -- 表示・非表示 --
		void ShowHideUI(const bool ShowActive);

		StageType m_Type;
		bool m_Active;

		bool m_IsSent;

		map<wstring, shared_ptr<FlashingUI>> m_UIMap;

		wstring m_TexKey;

		shared_ptr<FlashingUI> m_CurrntUI;
	};

	//----------------------------------------------------------------------------
	//スプライト
	//----------------------------------------------------------------------------
	class AnimSprite :public GameObject
	{
	public:
		AnimSprite(const shared_ptr<Stage>& StagePtr, const wstring TexKey, const bool ActiveAnime = false, const bool Loop = false)
			:GameObject(StagePtr), m_TexKey(TexKey), m_IsLoop(Loop), m_IsActive(ActiveAnime)
		{
			float Sizex = (float)App::GetApp()->GetGameWidth() / 2.0f;
			float Sizey = (float)App::GetApp()->GetGameHeight() / 2.0f;

			m_vertices.clear();
			m_indices.clear();
			m_vertices.push_back(VertexPositionColorTexture(Vec3(-Sizex, Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 0.0f)));
			m_vertices.push_back(VertexPositionColorTexture(Vec3(Sizex, Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)));
			m_vertices.push_back(VertexPositionColorTexture(Vec3(-Sizex, -Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)));
			m_vertices.push_back(VertexPositionColorTexture(Vec3(Sizex, -Sizey, 0), Col4(1.0f, 1.0f, 1.0, 1.0f), Vec2(1.0f, 1.0f)));

			m_indices = { 0, 1, 2, 1, 3, 2 };

		}
		virtual ~AnimSprite() {}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		wstring m_TexKey;

		bool m_IsActive = false;
		bool m_IsLoop = false;

		float m_vol = 1.0f;
		float m_TotalTime = 0.0f;
	};

	//----------------------------------------------------------------------------
	//フェードクラス
	//----------------------------------------------------------------------------
	enum class FadeType
	{
		FadeIn,
		FadeOut
	};

	class FadeObj : public GameObject
	{
	public:
		//-- 構築 --
		FadeObj(const shared_ptr<Stage>& StagePtr,const FadeType Type) 
			:GameObject(StagePtr),m_CurrntType(Type)
		{
			float Sizex = (float)App::GetApp()->GetGameWidth() / 2.0f;
			float Sizey = (float)App::GetApp()->GetGameHeight() / 2.0f;

			m_vertices.clear();
			m_indices.clear();
			m_vertices.push_back(VertexPositionColor(Vec3(-Sizex, Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f)));
			m_vertices.push_back(VertexPositionColor(Vec3(Sizex, Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f)));
			m_vertices.push_back(VertexPositionColor(Vec3(-Sizex, -Sizey, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f)));
			m_vertices.push_back(VertexPositionColor(Vec3(Sizex, -Sizey, 0), Col4(1.0f, 1.0f, 1.0, 1.0f)));

			m_indices = { 0, 1, 2, 1, 3, 2 };
		}

		//-- 破棄 --
		virtual ~FadeObj() {}

		//-- 初期化 --
		void OnCreate()override;

		//-- 更新処理 --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>& event);

	private:
		vector<VertexPositionColor> m_vertices;
		vector<uint16_t> m_indices;

		FadeType m_CurrntType;

		shared_ptr<ObjectInterface> m_Resiver;

		bool m_ActiveFade;
		wstring m_EventMsgStr;
	};
}