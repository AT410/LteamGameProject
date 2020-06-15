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
		GetComponent<Transform>()->SetPosition(GetStartPos());

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

		if (m_EventStr == L"ToGameStage"||m_EventStr == L"ToStageSelectStage")
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

		GetComponent<Transform>()->SetPosition(GetStartPos());

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
		if (m_EventStr == L"ToStageSelectStage"&& m_AreaNum >= 0)
		{
			GameManager::GetManager()->SetAreaNumber(m_AreaNum);
		}

		if (m_EventStr == L"ToGameStage"&&m_StageNum>=0)
		{
			GameManager::GetManager()->SetStageNumber(m_StageNum);
		}
		GameManager::GetManager()->SetStartCameraActive(false);
		PostEvent(1.0f, GetThis<ObjectInterface>(),L"Fade", m_EventStr,L"FadeOut");
	}

	//-----------------------------------------------------------------------------
	//UIコントローラー
	//-----------------------------------------------------------------------------
	void UIController::OnCreate()
	{
		GetStage()->SetSharedGameObject(L"UIController", GetThis<UIController>());
		m_CurrntUI = nullptr;
		
		if (m_Type == StageType::GameStage)
		{
			GetStage()->CreateSharedObjectGroup(L"GameStageUI");
		}

	}

	void UIController::OnUpdate()
	{

		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		if (Cont.wPressedButtons == XINPUT_GAMEPAD_START)
			m_Active = !m_Active;

		if (m_Active||m_Type != StageType::GameStage) 
		{
			if (m_Type == StageType::GameStage)
				ShowHideUI(true);
			SetCurrntUI();

			OperationUI(Cont);

			//入力Handler
			m_handler.PushHandler(GetThis<UIController>());

			// -- UIがアクティブの時は、更新を止める --
			GameManager::GetManager()->SetUpdateActive(false);
		}
		else
		{
			// -- UIを非表示に --
			ShowHideUI(false);
			GameManager::GetManager()->SetUpdateActive(true);
		}
	}

	void UIController::OperationUI(const CONTROLER_STATE Cont)
	{
		if (m_IsSent)
			return;

		WORD Dpad = m_handler.CovertToDPAD(Vec2(Cont.fThumbLX, Cont.fThumbLY));

		if (m_handler.GetUpArrow(Dpad))
		{
			wstring UpStr = m_CurrntUI->GetUpStr();
			ChangeActiveUI(UpStr);
		}
		else if (m_handler.GetDownArrow(Dpad))
		{
			wstring DownStr = m_CurrntUI->GetDownStr();
			ChangeActiveUI(DownStr);
		}

		if (m_handler.GetLeftArrow(Dpad))
		{
			wstring LeftStr = m_CurrntUI->GetLeftStr();
			ChangeActiveUI(LeftStr);
		}
		else if (m_handler.GetRightArrow(Dpad))
		{
			wstring RightStr = m_CurrntUI->GetRightStr();
			ChangeActiveUI(RightStr);
		}
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
		if (m_IsSent)
			return;
		App::GetApp()->GetXAudio2Manager()->Start(L"AGree_SD", 0, 0.5f);
		//対応するイベントを発生させる
		m_CurrntUI->SetFlashingSpeed(2.5f);
		m_CurrntUI->StartEvent();
		m_IsSent = true;
	}

	void UIController::OnPushB()
	{
		if (m_IsSent)
			return;

		wstring BackEventMsg;
		switch (m_Type)
		{
		case basecross::StageType::TitleStage:
			return;
			break;
		case basecross::StageType::LoadStage:
			return;
			break;
		case basecross::StageType::DataSelectStage:
			return;
			break;
		case basecross::StageType::AreaSelectStage:
			BackEventMsg = L"ToTitleStage";
			break;
		case basecross::StageType::StageSelectStage:
			BackEventMsg = L"ToAreaSelectStage";
			break;
		case basecross::StageType::GameStage:
			m_Active = false;
			return;
			break;
		case basecross::StageType::EndingStage:
			return;
			break;
		default:
			break;
		}

		App::GetApp()->GetXAudio2Manager()->Start(L"AGree_SD", 0, 0.5f);
		PostEvent(0.0f, GetThis<UIController>(), L"Fade", BackEventMsg, L"FadeOut");
		m_IsSent = true;
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

	// -- 表示・非表示 --
	void UIController::ShowHideUI(const bool ShowActive)
	{
		// -- 対象となるUIを操作 --
		auto GroupPtr = GetStage()->GetSharedObjectGroup(L"GameStageUI",false);
		if (GroupPtr)
		{
			for (auto ptr : GroupPtr->GetGroupVector())
			{
				ptr.lock()->SetDrawActive(ShowActive);
			}
		}
	}

	//----------------------------------------------------------------------------
	//スプライトクラス
	//----------------------------------------------------------------------------
	void AnimSprite::OnCreate()
	{
		auto DrawComp = AddComponent<PCTSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColorTexture>(m_vertices, m_indices);
		DrawComp->SetTextureResource(m_TexKey, false);

		float Alpha = m_IsActive ? 0.0f : 1.0f;

		DrawComp->SetDiffuse(Col4(1, 1, 1, Alpha));
		SetAlphaActive(true);
	}

	void AnimSprite::OnUpdate()
	{
		if (!m_IsLoop)
		{
			if (m_IsActive)
			{
				auto DrawComp = GetComponent<PCTSpriteDraw>();
				auto Diffuse = DrawComp->GetDiffuse();
				if (Diffuse.w >= 1.0f)
				{
					m_vol = -1.0f;
				}
				if (Diffuse.w < 0.0f)
				{
					m_IsActive = false;
					GetStage()->GetSharedGameObject<Player>(L"Player")->SetState(PlayerState::Excute);
				}
				Diffuse.w += m_vol * App::GetApp()->GetElapsedTime();
				DrawComp->SetDiffuse(Diffuse);
			}
		}
		else
		{
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			PtrDraw->SetDiffuse(col);

		}
	}

	//----------------------------------------------------------------------------
	//フェードクラス
	//----------------------------------------------------------------------------
	void FadeObj::OnCreate()
	{
		auto DrawComp = AddComponent<PCSpriteDraw>();
		DrawComp->CreateMesh<VertexPositionColor>(m_vertices, m_indices);

		m_ActiveFade = true;

		switch (m_CurrntType)
		{
		case basecross::FadeType::FadeIn:
			DrawComp->SetDiffuse(Col4(0,0,0,1));
			break;
		case basecross::FadeType::FadeOut:
			DrawComp->SetDiffuse(Col4(0));
			break;
		default:
			break;
		}

		SetAlphaActive(true);

		App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(L"Fade", GetThis<FadeObj>());
	}

	void FadeObj::OnUpdate()
	{
		if (m_ActiveFade)
		{
			auto DrawComp = GetComponent<PCSpriteDraw>();
			Col4 Diffuse = DrawComp->GetDiffuse();
			switch (m_CurrntType)
			{
			case basecross::FadeType::FadeIn:
				Diffuse.w -= App::GetApp()->GetElapsedTime();
				if (Diffuse.w < 0.0f)
				{
					Diffuse.w = 0.0f;
					m_ActiveFade = false;
				}
				break;
			case basecross::FadeType::FadeOut:
				Diffuse.w += App::GetApp()->GetElapsedTime();
				if (Diffuse.w > 1.0f)
				{
					Diffuse.w = 1.0f;
					m_ActiveFade = false;
					// -- 暗転したらイベント飛ばす --
					PostEvent(0.0f, GetThis<FadeObj>(), m_Resiver, m_EventMsgStr);
					if (m_EventMsgStr == L"ReStart") {
						auto DrawComp = GetComponent<PCSpriteDraw>();
						DrawComp->SetDiffuse(Col4(0, 0, 0, 1));
						m_CurrntType = FadeType::FadeIn;
						m_ActiveFade = true;
					}
				}
				break;
			default:
				break;
			}

			DrawComp->SetDiffuse(Diffuse);			
		}
	}

	void FadeObj::OnEvent(const shared_ptr<Event>&event)
	{
		if (event->m_MsgStr2 == L"FadeIn")
		{
			auto DrawComp = GetComponent<PCSpriteDraw>();
			DrawComp->SetDiffuse(Col4(0, 0, 0, 1));
			m_EventMsgStr = event->m_MsgStr;
			m_ActiveFade = true;
		}
		else if (event->m_MsgStr2 == L"FadeOut")
		{
			auto DrawComp = GetComponent<PCSpriteDraw>();
			DrawComp->SetDiffuse(Col4(0, 0, 0, 0));
			m_EventMsgStr = event->m_MsgStr;
			m_ActiveFade = true;
			m_CurrntType = FadeType::FadeOut;
			auto SendPtr = event->m_Sender.lock();
			if (SendPtr)
			{
				if (event->m_MsgStr == L"ReStart")
				{
					m_Resiver = SendPtr;
				}
				else 
					m_Resiver = App::GetApp()->GetScene<Scene>();
			}
		}
		else if (event->m_MsgStr2 == L"FadeOutGoal")
		{
			auto DrawComp = GetComponent<PCSpriteDraw>();
			DrawComp->SetDiffuse(Col4(1, 1, 1, 0));
			m_EventMsgStr = event->m_MsgStr;
			m_ActiveFade = true;
			m_CurrntType = FadeType::FadeOut;

			m_Resiver = App::GetApp()->GetScene<Scene>();
		}
	}
}