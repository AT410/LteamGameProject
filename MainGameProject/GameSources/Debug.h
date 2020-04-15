/*!
@breif �f�o�b�N�p�N���X
@name �쐬�ҁF�����B��
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	class DebugSprite :public GameObject
	{
	public:
		DebugSprite(const shared_ptr<Stage>& StagePtr,const wstring& TexKey)
			:GameObject(StagePtr), m_TexKey(TexKey)
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
		virtual ~DebugSprite(){}

		void OnCreate()override;

	private:
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		wstring m_TexKey;
	};

	//����e�X�g
	class ContTest :public ObjectBase,public PawnBase<ContTest>
	{
	public:
		ContTest(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr),PawnBase()
		{}
		virtual ~ContTest(){}

		void OnCreate()override;
		
		void OnUpdate()override;

		void OnPushA()override;
	};

	//�X�e�[�W�z�u�e�X�g
	class StageTest :public ObjectBase
	{
	public:
		StageTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~StageTest() {}

		void OnCreate()	override;
	};

	//�X�e�[�W�z�u�e�X�g
	class GoalTest :public ObjectBase
	{
	public:
		GoalTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~GoalTest() {}

		void OnCreate()	override;
		
		void OnUpdate()override;

		void SetGoal(bool IsGoal) { m_IsGoal = IsGoal; }

	private:
		bool m_IsGoal;
		float m_count;
	};

}