/*!
@breif	�v���W�F�N�g��`��UI
@name	�쐬�ҁF�����B��
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
}