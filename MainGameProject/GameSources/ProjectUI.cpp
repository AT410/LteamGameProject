/*!
@breif プロジェクト定義のUI実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//----------------------------------------------------------------------------
	//UIBase::Impl構造体
	//----------------------------------------------------------------------------
	struct UIBase::Impl
	{
		Vec3 m_StartPos;
		wstring m_TexKey;
		
		float m_Width;
		float m_Height;

		Vec3 GetStartPosition()const { return m_StartPos; }
		wstring GetTexKey()const { return m_TexKey; }
		float GetUIWidth()const { return m_Width; }
		float GetUIHeight()const { return m_Height; }

		Impl()
			:m_StartPos(Vec3(0)),m_TexKey(L"TEST_TX"),m_Height(100.0f),m_Width(100.0f)
		{}

		Impl(IXMLDOMNodePtr pNode)
		{
			auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
			m_TexKey = XmlDocReader::GetAttribute(pNode, L"TexKey");
			auto WidthStr = XmlDocReader::GetAttribute(pNode, L"Width");
			auto HeightStr = XmlDocReader::GetAttribute(pNode, L"Height");

			//トークン分け
			vector<wstring> Tokens;

			Util::WStrToTokenVector(Tokens, PosStr, L',');

			m_StartPos = Vec3(	(float)_wtof(Tokens[0].c_str()),
								(float)_wtof(Tokens[1].c_str()),
								0.0f);

			m_Width = (float)_wtof(WidthStr.c_str());

			m_Height = (float)_wtof(HeightStr.c_str());

			 
		}
	};

	//----------------------------------------------------------------------------
	//UIBaseの実体
	//----------------------------------------------------------------------------
	UIBase::UIBase(const shared_ptr<Stage>&StagePtr)
		:GameObject(StagePtr), pImpl(make_unique<UIBase::Impl>())
	{

	}

	UIBase::UIBase(const shared_ptr<Stage>&StagePtr,IXMLDOMNodePtr pNode)
		: GameObject(StagePtr), pImpl(make_unique<UIBase::Impl>(pNode))
	{

	}

	UIBase::~UIBase()
	{

	}

	//ゲッター
	Vec3 UIBase::GetStartPos()const { return pImpl->GetStartPosition(); }
	wstring UIBase::GetTexKey()const { return pImpl->GetTexKey(); }
	float UIBase::GetUIWidth()const { return pImpl->GetUIWidth(); }
	float UIBase::GetUIHeight()const { return pImpl->GetUIHeight(); }


	//----------------------------------------------------------------------------
	//通常UI：（常時表示）の実体
	//----------------------------------------------------------------------------
	// -- 初期化 --
	void NormalUI::OnCreate()
	{
		// -- メッシュの作成 --
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		Vec3 StartPos = GetStartPos();
		float halfWidth = GetUIWidth() / 2.0f;
		float halfHeight = GetUIHeight() / 2.0f;

		vector<VertexPositionTexture>vertices =
		{
			{Vec3(-halfWidth,+halfHeight,0.0f),Vec2(0		,0)},
			{Vec3(+halfWidth,+halfHeight,0.0f),Vec2(tipSize.x,0)},
			{Vec3(-halfWidth,-halfHeight,0.0f),Vec2(0		,tipSize.y)},
			{Vec3(+halfWidth,-halfHeight,0.0f),Vec2(tipSize.x,tipSize.y)}
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};

		// -- 描画設定 --
		auto DrawComp = AddComponent<PTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionTexture>(vertices, indices);
		DrawComp->SetTextureResource(GetTexKey());

		// -- 配置設定 --
		auto TransComp = GetComponent<Transform>();
		auto pos = GetStartPos();
		pos.z = +0;
		TransComp->SetPosition(pos);

		// -- 透過処理 --
		SetAlphaActive(true);
	}
	//----------------------------------------------------------------------------
	//点滅UI:(選択可能UI)の実体
	//----------------------------------------------------------------------------
	FlashingUI::FlashingUI(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:UIBase(StagePtr, pNode),m_FlashingSpeed(0.5f),m_ActiveFlashing(false)
	{
		//追加情報の取得
		m_Up = XmlDocReader::GetAttribute(pNode, L"UpKey");
		m_Down = XmlDocReader::GetAttribute(pNode, L"DownKey");
		m_Left = XmlDocReader::GetAttribute(pNode, L"LeftKey");
		m_Right = XmlDocReader::GetAttribute(pNode, L"RightKey");
		m_MyKey = XmlDocReader::GetAttribute(pNode, L"MyIndexKey");
		m_EventStr = XmlDocReader::GetAttribute(pNode, L"EventKey");

		if (m_EventStr == L"ToGameStage")
		{
			wstring StageNumStr = XmlDocReader::GetAttribute(pNode, L"StageNumber");
			wstring AreaNumStr = XmlDocReader::GetAttribute(pNode, L"AreaNumber");

			m_AreaNum = (int)_wtoi(AreaNumStr.c_str());
			m_StageNum = (int)_wtoi(StageNumStr.c_str());
		}
	}

	void FlashingUI::OnCreate()
	{
		Col4 color(1);
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		Vec3 StartPos = GetStartPos();
		float halfWidth = GetUIWidth()/2.0f;
		float halfHeight = GetUIHeight()/2.0f;

		vector<VertexPositionColorTexture>vertices =
		{
			{Vec3(-halfWidth,+halfHeight,1.0f), color,Vec2(0		,0)},
			{Vec3(+halfWidth,+halfHeight,1.0f), color,Vec2(tipSize.x,0)},
			{Vec3(-halfWidth,-halfHeight,1.0f), color,Vec2(0		,tipSize.y)},
			{Vec3(+halfWidth,-halfHeight,1.0f), color,Vec2(tipSize.x,tipSize.y)},
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};

		auto DrawComp = AddComponent<PCTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColorTexture>(vertices, indices);
		DrawComp->SetTextureResource(GetTexKey());

		auto TransComp = GetComponent<Transform>();
		auto pos = GetStartPos();
		pos.z = 1;
		TransComp->SetPosition(pos);

		SetAlphaActive(true);

		//コントロールに追加
		GetStage()->GetSharedGameObject<UIController>(L"UIController")->AddPawnUI(m_MyKey, GetThis<FlashingUI>());
	}

	void FlashingUI::OnUpdate()
	{
		auto DrawComp = GetComponent<PCTSpriteDraw>();
		if (m_ActiveFlashing)
		{
			float ElapsedTime = m_FlashingSpeed*App::GetApp()->GetElapsedTime();

			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			Col4 col(0.5f,1.0f, 1.0f, 1.0f);
			col.w = abs(sin(m_TotalTime));
			DrawComp->SetDiffuse(col);
		}
		else
		{
			DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
			m_TotalTime = 0;
		}

	}

	void FlashingUI::StartEvent()
	{
		if (m_EventStr == L"ToGameStage")
		{
			GameManager::GetManager()->SetStagePair(m_AreaNum, m_StageNum);
		}

		PostEvent(1.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), m_EventStr);
	}

	//----------------------------------------------------------------------------
	//スイッチングUIの実体
	//----------------------------------------------------------------------------
	void SwitchingUI::OnCreate()
	{
		// -- メッシュの作成 --
		Vec2 tipSize = Vec2(1.0f, 1.0f);

		Vec3 StartPos = GetStartPos();
		float halfWidth = GetUIWidth() / 2.0f;
		float halfHeight = GetUIHeight() / 2.0f;

		vector<VertexPositionTexture>vertices =
		{
			{Vec3(-halfWidth,+halfHeight,0.0f),Vec2(0		,0)},
			{Vec3(+halfWidth,+halfHeight,0.0f),Vec2(tipSize.x,0)},
			{Vec3(-halfWidth,-halfHeight,0.0f),Vec2(0		,tipSize.y)},
			{Vec3(+halfWidth,-halfHeight,0.0f),Vec2(tipSize.x,tipSize.y)}
		};

		vector<uint16_t> indices =
		{
			0,1,2,
			2,1,3,
		};

		// -- 描画設定 --
		auto DrawComp = AddComponent<PTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionTexture>(vertices, indices);
		DrawComp->SetTextureResource(GetTexKey());

		// -- 配置設定 --
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(GetStartPos());

	}

	void TestUI::OnCreate()
	{
		auto DrawComp = AddComponent<PTStaticDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionTexture>(m_vertices, m_indices);
		DrawComp->SetOriginalMeshUse(true);
		DrawComp->SetTextureResource(m_texkey);
		auto TransComp = GetComponent<Transform>();
		TransComp->SetPosition(m_Pos);

		SetAlphaActive(true);
	}

	void TestUI::OnUpdate()
	{
		
		auto DrawComp = GetComponent<PTStaticDraw>();
		if (m_ActiveFlag)
		{
			float ElapsedTime = App::GetApp()->GetElapsedTime();

			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			Col4 col(1.0, 0.0f, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			DrawComp->SetDiffuse(col);
		}
		else
		{
			DrawComp->SetDiffuse(Col4(1, 1, 1, 1));
			m_TotalTime = 0;
		}

	}

	//-----------------------------------------------------------------------------
	//UIコントローラー
	//-----------------------------------------------------------------------------
	void UIController::OnCreate()
	{
		GetStage()->SetSharedGameObject(L"UIController", GetThis<UIController>());
		m_CurrntUI = nullptr;
	}

	void UIController::OnUpdate()
	{
		SetCurrntUI();

		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		if (Cont.wPressedButtons == XINPUT_GAMEPAD_DPAD_UP)
		{
			wstring UpStr = m_CurrntUI->GetUpStr();
			ChangeActiveUI(UpStr);
		}
		else if (Cont.wPressedButtons == XINPUT_GAMEPAD_DPAD_DOWN)
		{
			wstring DownStr = m_CurrntUI->GetDownStr();
			ChangeActiveUI(DownStr);
		}

		if (Cont.wPressedButtons == XINPUT_GAMEPAD_DPAD_LEFT)
		{
			wstring LeftStr = m_CurrntUI->GetLeftStr();
			ChangeActiveUI(LeftStr);
		}
		else if (Cont.wPressedButtons == XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			wstring RightStr = m_CurrntUI->GetRightStr();
			ChangeActiveUI(RightStr);
		}

		//入力Handler
		m_handler.PushHandler(GetThis<UIController>());
	}

	void UIController::SetCurrntUI()
	{
		if (m_CurrntUI == nullptr)
		{
			auto it = m_UIMap.begin();
			if (it != m_UIMap.end())
			{
				m_CurrntUI = (*it).second;
				m_CurrntUI->ChangeActive(true);
			}
		}
	}

	void UIController::OnPushA()
	{
		//対応するイベントを発生させる
		m_CurrntUI->SetFlashingSpeed(2.5f);
		m_CurrntUI->StartEvent();
	}
	void UIController::ChangeActiveUI(const wstring& Key)
	{
		auto it = m_UIMap.find(Key);
		if (it != m_UIMap.end())
		{
			m_CurrntUI->ChangeActive(false);

			m_CurrntUI = m_UIMap[Key];

			m_CurrntUI->ChangeActive(true);
		}
	}
}