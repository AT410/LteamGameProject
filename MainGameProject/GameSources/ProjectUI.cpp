/*!
@breif プロジェクト定義のUI実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	struct UIBase::Impl
	{
		Impl()
		{

		}
	};

	UIBase::UIBase(const shared_ptr<Stage>&StagePtr)
		:GameObject(StagePtr),pImpl(make_unique<UIBase::Impl>())
	{

	}

	UIBase::~UIBase()
	{

	}

	void TestUI::OnCreate()
	{
		auto DrawComp = AddComponent<PTStaticDraw>();
		DrawComp->CreateOriginalMesh<VertexPositionTexture>(m_vertices, m_indices);
		DrawComp->SetOriginalMeshUse(true);
		
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
		}
	}

	UIController::UIController(const shared_ptr<Stage>&StagePtr)
		:GameObject(StagePtr), m_UIMap()
	{

	}

	void UIController::OnCreate()
	{
		auto Ptr = GetStage()->AddGameObject<TestUI>(Vec3(0, 2, 0), L"4", L"2", L"3", L"3");
		m_UIMap[L"1"] = Ptr;
		m_CurrntUI = Ptr;

		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(0, 0, 0), L"1", L"3", L"4", L"4");
		m_UIMap[L"2"] = Ptr;
		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(2, 2, 0), L"2", L"4", L"1", L"1");
		m_UIMap[L"3"] = Ptr;
		Ptr = GetStage()->AddGameObject<TestUI>(Vec3(2,0, 0), L"3", L"1", L"2", L"2");
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