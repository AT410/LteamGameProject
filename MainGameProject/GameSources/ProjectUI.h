/*!
@breif	プロジェクト定義のUI
@name	作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	class UIBase :public GameObject
	{
	public:
		UIBase(const shared_ptr<Stage>&StagePtr);
		
		virtual ~UIBase();

		void OnCreate()override = 0;

		virtual void OnUpdate()override {}

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
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

	class UIController :public GameObject
	{
	public:
		UIController(const shared_ptr<Stage>&StagePtr, const wstring& TexKey);

		virtual ~UIController(){}

		void OnCreate()override;

		void OnUpdate()override;

		void ChangeActiveUI(const wstring& Key);

		map<wstring, shared_ptr<TestUI>> m_UIMap;

		wstring m_TexKey;

		shared_ptr<TestUI> m_CurrntUI;
	};
}