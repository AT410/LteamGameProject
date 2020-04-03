#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class DebugTest :public GameObject
	{
	public:
		DebugTest(const shared_ptr<Stage>&StagePtr)
			:GameObject(StagePtr)
		{}
		


		void OnCreate()override;
		void OnUpdate()override;
	};
}