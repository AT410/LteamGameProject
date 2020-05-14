/*!
@breif デバック用クラス
@name 作成者：阿部達哉
*/
#pragma once
#include "stdafx.h"

namespace basecross
{
	class RockTest;

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

	class ShaderTest :public GameObject
	{
	public:
		ShaderTest(const shared_ptr<Stage>& StagePtr, const wstring& TexKey)
			:GameObject(StagePtr), m_TexKey(TexKey), m_TotalTime(0.0f)
		{
			float Sizex = (float)App::GetApp()->GetGameWidth() / 2.0f;
			float Sizey = (float)App::GetApp()->GetGameHeight() / 2.0f;

			m_vertices.clear();
			m_indices.clear();
			m_vertices.push_back(VertexPositionTexture(Vec3(-Sizex, Sizey, 0),Vec2(0.0f, 0.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(Sizex, Sizey, 0), Vec2(1.0f, 0.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(-Sizex, -Sizey, 0), Vec2(0.0f, 1.0f)));
			m_vertices.push_back(VertexPositionTexture(Vec3(Sizex, -Sizey, 0),Vec2(1.0f, 1.0f)));

			m_indices = { 0, 1, 2, 2, 1, 3};
		}
		virtual ~ShaderTest() {}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		vector<VertexPositionTexture> m_vertices;
		vector<uint16_t> m_indices;
		wstring m_TexKey;

		float m_TotalTime;
	};


	class AnimSpriteTest :public GameObject
	{
	public:
		AnimSpriteTest(const shared_ptr<Stage>& StagePtr, const int& num,bool Loop = false)
			:GameObject(StagePtr),m_TexNum(num),m_IsLoop(Loop)
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

			wstring Leng[6] = { L"Stage1_TX",L"Stage2_TX",L"Stage3_TX",L"Stage4_TX",L"Stage5_TX",L"WAIT_TX" };
			m_TexKey = Leng[num];
		}
		virtual ~AnimSpriteTest() {}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		vector<VertexPositionColorTexture> m_vertices;
		vector<uint16_t> m_indices;
		int m_TexNum;
		wstring m_TexKey;
		bool m_IsActived=false;
		bool m_IsLoop = false;
		float m_vol = 1.0f;
		float m_TotalTime = 0.0f;
	};


	//操作テスト
	class ContTest :public ObjectBase,public PawnBase<ContTest>
	{
	public:
		ContTest(const shared_ptr<Stage>&StagePtr,const wstring& StageStr )
			:ObjectBase(StagePtr),PawnBase(), m_StageStr(StageStr)
		{}
		virtual ~ContTest(){}

		void OnCreate()override;
		
		void OnUpdate()override;

		void OnPushA()override;

	private:
		wstring m_StageStr;
	};

	//ステージ配置テスト
	class StageTest :public ObjectBase
	{
	public:
		StageTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~StageTest() {}

		void OnCreate()	override;
	};

	//--------------------------------------------------------------------------------------
	///	数字のスクエア
	//--------------------------------------------------------------------------------------
	class NumberSquare : public GameObject {
		weak_ptr<RockTest> m_SeekObject;
		//このオブジェクトのみで使用するスクエアメッシュ
		shared_ptr<MeshResource> m_SquareMeshResource;
		//背番号
		size_t m_Number;

		Quat Billboard(const Vec3& Line) {
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f) {
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

	public:
		//構築と破棄
		NumberSquare(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<RockTest>& SeekObjectPtr, size_t Number);
		virtual ~NumberSquare();
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
	};


	//ステージ配置テスト
	class RockTest :public ObjectBase
	{
	public:
		RockTest(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~RockTest() {}

		void OnCreate()	override;

	private:
		size_t num = 0;
	};



	//ステージ配置テスト
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