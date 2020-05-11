/*!
@breif	プロジェクト定義の追加関数・機能
@name	作成者：阿部達哉
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//---------------------------------
	//コントローラー操作用ハンドラ
	//---------------------------------
	template<typename T>
	struct InputHandler
	{
		//Vec2 m_LStickVol;//Lスティック
		//Vec2 m_RStickVol;//Rスティック

		//bool m_ConvertStickToDbadActoive;

		void PushHandler(const shared_ptr<T>& Obj)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];
			auto KeyInput = InputDevice.GetKeyState();
			if (ContInput.bConnected)
			{
				/*!
				@breif　決定ボタンorジャンプ
				*/
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_A)
				{
					Obj->OnPushA();
				}

				//スティック押込み
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_RIGHT_THUMB)
				{
					Obj->OnPushR3();
				}

				if (ContInput.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER)
				{
					Obj->OnPushLB();
				}
				else {
					Obj->OnRemoveLB();
				}
				if (ContInput.wPressedButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Obj->OnPushRB();
				}
			}
		}
	};

	//---------------------------------
	//ステージビルダーの作成
	//---------------------------------
	class StageBulider
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		StageBulider();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StageBulider();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クリエーターを登録する
		@tparam	T	登録する型（ゲームオブジェクトの派生）
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void Register(const wstring& ClsName) {
			GetCreatorMap()[ClsName] = shared_ptr<GameObjectCreatorXML<T>>(new GameObjectCreatorXML<T>());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	型の識別名を使ってゲームオブジェクトを構築する
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@param[in]	StagePtr	所属するステージ
		@param[in]	pNode	XMLノード
		@return	作成したゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> CreateFromXML(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XMLからゲームステージを構築する
		@param[in]	StagePtr	所属するステージ
		@param[in]	XMLFileName	XMLファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StageBuild(const shared_ptr<Stage>& StagePtr, const wstring& XMLFileName);
	private:
		map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GetCreatorMap() const;

		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}