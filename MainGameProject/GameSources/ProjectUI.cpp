/*!
@breif プロジェクト定義のUI実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
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

		Impl(){}

		Impl(IXMLDOMNodePtr pNode)
		{
			auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
			//auto TexStr = XmlDocReader::GetAttribute(pNode, L"TexKey");
			auto WidthStr = XmlDocReader::GetAttribute(pNode, L"Width");
			auto HeightStr = XmlDocReader::GetAttribute(pNode, L"Height");

			//トークン分け
			vector<wstring> Tokens;

			Util::WStrToTokenVector(Tokens, PosStr, L',');

			m_StartPos = Vec3(	(float)_wtof(Tokens[0].c_str()),
								(float)_wtof(Tokens[1].c_str()),
								(float)_wtof(Tokens[2].c_str()));

			m_Width = (float)_wtof(WidthStr.c_str());

			m_Height = (float)_wtof(HeightStr.c_str());
		}
	};

	UIBase::UIBase(const shared_ptr<Stage>&StagePtr)
		:GameObject(StagePtr),pImpl(make_unique<UIBase::Impl>())
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

	UIController::UIController(const shared_ptr<Stage>&StagePtr, const wstring& TexKey)
		:GameObject(StagePtr), m_UIMap(), m_TexKey(TexKey)
	{

	}

	void UIController::OnCreate()
	{
		auto Ptr = GetStage()->AddGameObject<TestUI>(Vec3(-5, 5, 0), L"4", L"2", L"3", L"2", m_TexKey);
		m_UIMap[L"1"] = Ptr;
		m_CurrntUI = Ptr;
		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(-5, -5, 0), L"1", L"3", L"4", L"4", L"WATER_TX");
		m_UIMap[L"2"] = Ptr;
		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(5, 5, 0), L"2", L"4", L"1", L"1", L"TEST_TX");
		m_UIMap[L"3"] = Ptr;
		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(5,-5, 0), L"3", L"1", L"2", L"2", m_TexKey);
		m_UIMap[L"4"] = Ptr;
		
	}

	void UIController::OnUpdate()
	{

		auto Cont = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		m_CurrntUI->SetActiveFlag(true);

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
	}

	void UIController::ChangeActiveUI(const wstring& Key)
	{
		m_CurrntUI->SetActiveFlag(false);

		m_CurrntUI = m_UIMap[Key];

		m_CurrntUI->SetActiveFlag(true);
	}
}