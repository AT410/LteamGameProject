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

		WORD wDpad = 0;
		WORD wPressedDpad = 0;
		WORD wNowStick;
		WORD wLastStick = 0;
		bool m_ConvertStickToDbadActoive= false;
		bool m_ConvertStickToDbadActoiveDown = false;
		bool m_ConvertStickToDbadActoiveLeft = false;
		bool m_ConvertStickToDbadActoiveRight = false;

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

		WORD CovertToDPAD(const Vec2 Stick)
		{
			// -- 一フレーム前 --
			wLastStick = wDpad;

			wDpad = 0;
			if (Stick.x > 0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			else if (Stick.x < -0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_LEFT;
			}

			if (Stick.y > 0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_UP;
			}
			else if (Stick.y < -0.5f)
			{
				wDpad |= XINPUT_GAMEPAD_DPAD_DOWN;
			}

			if (wDpad != wLastStick && m_ConvertStickToDbadActoive)
				m_ConvertStickToDbadActoive = false;

			if (m_ConvertStickToDbadActoive)
			{
				return 0;
			}
			else
			{
				m_ConvertStickToDbadActoive = true;
				wLastStick = wDpad;
				return wDpad;
			}
		}

		bool GetUpArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_UP || DPAD == XINPUT_GAMEPAD_DPAD_UP)
			{
				return true;
			}

			return false;
		}

		bool GetDownArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_DOWN || DPAD == XINPUT_GAMEPAD_DPAD_DOWN)
			{
				return true;
			}

			return false;
		}

		bool GetRightArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_RIGHT || DPAD == XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				return true;
			}

			return false;
		}

		bool GetLeftArrow(const WORD DPAD)
		{
			auto InputDevice = App::GetApp()->GetInputDevice();
			auto ContInput = InputDevice.GetControlerVec()[0];

			if (ContInput.wPressedButtons == XINPUT_GAMEPAD_DPAD_LEFT || DPAD & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				return true;
			}

			return false;
		}
	};

	class StageBase;

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
		void StageBuild(const shared_ptr<StageBase>& StagePtr, const wstring& XMLFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XMLからUIセットを構築する
		@param[in]	StagePtr	所属するステージ
		@param[in]	XMLFileName	XMLファイル名
		@param[in]	DefaultDrawActive　生成時描画するかどうか。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UISetBuild(const shared_ptr<StageBase>& StagePtr, const wstring& XMLFileName, const bool DefaultDrawActive);
	private:
		map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GetCreatorMap() const;

		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}