/*!
@breif	プロジェクト定義のUI
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
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

	//----------------------------------------------------------------------------
	//スイッチングUI
	//----------------------------------------------------------------------------
	class SwitchingUI : public UIBase
	{
	public:
		//-- 構築 --
		SwitchingUI(const shared_ptr<Stage>& StagePtr)
			:UIBase(StagePtr)
		{
		}

		//-- 破棄 --
		virtual ~SwitchingUI() {}

		//-- 初期化 --
		void OnCreate()override;

		//-- 更新処理 --
		void OnUpdate()override {}

	private:

	};

	class TestUI :public GameObject
	{
	public:
		TestUI(const shared_ptr<Stage>& StagePtr,const Vec3& pos, 
			const wstring& Up, const wstring& Down, const wstring& Right, const wstring& Left,const wstring& TexKey)
			:GameObject(StagePtr), m_TotalTime(0.0f),m_Pos(pos),m_Up(Up),m_Down(Down),m_Right(Right),m_Left(Left), m_texkey(TexKey)
		{
			float Sizex = 10/ 2.0f;
			float Sizey = 10/ 2.0f;

			m_vertices.clear();
			m_indices.clear();
			m_vertices.push_back(VertexPositionTexture(Vec3(-Sizex, Sizey, 0), Vec2(0.0f, 0.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(Sizex, Sizey, 0), Vec2(1.0f, 0.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(-Sizex, -Sizey, 0), Vec2(0.0f, 1.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(Sizex, -Sizey, 0), Vec2(1.0f, 1.0f)));

			m_indices = { 0, 1, 2, 2, 1, 3 };
		}
		virtual ~TestUI() {}

		void OnCreate()override;

		void OnUpdate()override;

		void SetActiveFlag(bool b) { m_ActiveFlag = b; }

		wstring GetUpStr() { return m_Up; }
		wstring GetDownStr() { return m_Down; }
		wstring GetRightStr() { return m_Right; }
		wstring GetLeftStr() { return m_Left; }

	private:
		vector<VertexPositionTexture> m_vertices;
		vector<uint16_t> m_indices;

		wstring m_Up, m_Down, m_Right, m_Left,m_texkey;

		Vec3 m_Pos;

		float m_TotalTime;

		bool m_ActiveFlag = false;
	};

	class UIController :public GameObject,public PawnBase<UIController>
	{
	public:
		UIController(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr), PawnBase(), m_UIMap()
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

	private:
		map<wstring, shared_ptr<FlashingUI>> m_UIMap;

		wstring m_TexKey;

		shared_ptr<FlashingUI> m_CurrntUI;
	};
}