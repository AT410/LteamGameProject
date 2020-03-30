/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	struct Controller {
		float LX;
		float LY;
		float RX;
		float RY;
		bool A;
		bool LeftThumb;
		bool Dpad_Up;
		bool Dpad_Right;
		bool Dpad_Down;
		bool Dpad_Left;
		bool RT;
		bool LT;
		bool Start;

		Controller():
			LX(0.0f),
			LY(0.0f),
			RX(0.0f),
			RY(0.0f),
			A(false),
			Dpad_Up(false),
			Dpad_Right(false),
			Dpad_Down(false),
			Dpad_Left(false),
			LeftThumb(false),
			RT(false),
			LT(false),
			Start(false)

		{}
	};
	template<typename T>
	struct InputHandler {
		void PushHandle(int num, Controller& cntl) {
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[num].bConnected) {
				if (CntlVec[num].fThumbLX <= 0.5f || CntlVec[num].fThumbLX >= -0.5f ||
					CntlVec[num].fThumbLY <= 0.5f || CntlVec[num].fThumbLY >= -0.5f) {
					cntl.LX = CntlVec[num].fThumbLX;
					cntl.LY = CntlVec[num].fThumbLY;
				}
				else {
					cntl.LX = 0;
					cntl.LY = 0;
				}
				if (CntlVec[num].fThumbRX <= 0.5f || CntlVec[num].fThumbRX >= -0.5f ||
					CntlVec[num].fThumbRY <= 0.5f || CntlVec[num].fThumbRY >= -0.5f) {
					cntl.RX = CntlVec[num].fThumbRX;
					cntl.RY = CntlVec[num].fThumbRY;
				}
				else {
					cntl.RX = 0;
					cntl.RY = 0;
				}
				if (CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_A) {
					cntl.A = true;
				}
				if (CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB) {
					cntl.LeftThumb = true;
				}
				if (CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
					cntl.LT = true;
				}
				if (CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					cntl.RT = true;
				}
			}
		}

		void MemuController(int num, Controller& cntl) {
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			
			cntl.Dpad_Up = CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_DPAD_UP;
			
			cntl.Dpad_Right = CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
			
			cntl.Dpad_Down = CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			
			cntl.Dpad_Left = CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			
			cntl.Start = CntlVec[num].wPressedButtons & XINPUT_GAMEPAD_START;
			
			
		}
	};
	
	
}

//end basecross

