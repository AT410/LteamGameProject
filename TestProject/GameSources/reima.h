#pragma once
#include "stdafx.h"

namespace basecross
{
	class WaterWheel :public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float Time;
		float m_TotalTime;
		float m_Swap;

	public:
		WaterWheel(const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);

		virtual ~WaterWheel();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	//バルブ
	class Valve : public GameObject
	{
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float Time;
		float m_TotalTime;
		float m_Swap;
		float m_valveTime;

	public:
	Valve (const shared_ptr<Stage>&Stageptr,
			const Vec3& scale,
			const Vec3& Rotation,
			const Vec3& Position
		);

		virtual ~Valve();
		//初期化
		virtual void OnCreate() override;
		virtual void  OnUpdate() override;
		//virtual void OnUpdate() override;
	};

	class ValveGate : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		bool gateopen = false;
		float OpenSpeed = 2;
	public:
		ValveGate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~ValveGate();
		void setopenflg(bool active)
		{
			gateopen = active;
		}
		//初期化
		void ValveGateOpen();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};



	class ValveGate2: public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		bool gateopen = false;
		float OpenSpeed = 2;
	public:
		ValveGate2(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~ValveGate2();
		void setopenflg2(bool active)
		{
			gateopen = active;
		}
		//初期化
		void ValveGate2Open();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//--------------------------------------------------------------------------------------
	class TraceSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_changespeed;
		float m_changespeed2;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};


	class TraceSprite2 : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_changespeed;
		float m_changespeed2;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite2(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};



	class TraceSprite3 : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_changespeed;
		float m_changespeed2;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite3(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite3();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};


	class TraceSprite4 : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_changespeed;
		float m_changespeed2;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite4(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite4();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};







	class SelectSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		float m_changespeed;
		int m_state;
		float m_changespeed2;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		SelectSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SelectSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		//virtual void OnUpdate()override;
	};



		class Select : public GameObject
		{
		public:
			Select(const std::shared_ptr<Stage>& stage)
				: GameObject(stage)
			{
			}

			Vec3 GetPosition() const
			{
				return GetComponent<Transform>()->GetPosition();
			}


			// オブジェクトが生成されたときに一度だけ実行される関数
			void OnCreate() override;
			//void OnUpdate() override;
		};

}