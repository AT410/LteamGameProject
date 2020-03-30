/*!
@breif	オブジェクトの基底クラス実体
@name	作成者：阿部達哉
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	ObjectBase::ObjectBase(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:GameObject(StagePtr)
	{
		//配置データ、テクスチャキー、メッシュキー
	}
}