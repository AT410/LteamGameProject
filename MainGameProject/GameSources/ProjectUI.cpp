/*!
@breif �v���W�F�N�g��`��UI����
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
}