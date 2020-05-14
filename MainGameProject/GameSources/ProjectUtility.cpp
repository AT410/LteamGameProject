/*!
@breif	プロジェクト定義の追加関数・機能
@name	作成者：阿部達哉
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
//	struct StageBulider::Impl;
//	用途: Implイディオム
//--------------------------------------------------------------------------------------
	struct StageBulider::Impl {
		map<wstring, shared_ptr<GameObjectCreatorBaseXML> > m_CreatorMap;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	ステージビルダーToXML
	//--------------------------------------------------------------------------------------

	StageBulider::StageBulider() :
		pImpl(new Impl())
	{

	}
	StageBulider::~StageBulider() {}

	map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& StageBulider::GetCreatorMap() const {
		return pImpl->m_CreatorMap;
	}

	shared_ptr<GameObject>  StageBulider::CreateFromXML(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode) {
		auto it = pImpl->m_CreatorMap.find(ClsName);
		if (it == pImpl->m_CreatorMap.end()) {
			return nullptr;
		}
		else {
			auto ptr = (*it).second->Create(StagePtr, pNode);
			return ptr;
		}
	}

	void StageBulider::StageBuild(const shared_ptr<StageBase>& StagePtr, const wstring& XMLFileName) {
		try {
			//カメラ設定値
			Vec3 Eye, At;
			float Near, Far;

			Eye = Vec3(7.0f, 10.0f, -20.0f);
			At = Vec3(7.0f, 2.0f, 0.0f);
			Near = 0.5f;
			Far = 1000.0f;

			//ステージ選択値を取得
			auto Select = GameManager::GetManager()->GetStagePair();
			int SelectArea = Select.first;
			int SelectStage = Select.second;
			//XMLリーダー
			XmlDocReader Reader(XMLFileName);
			auto Nodes = Reader.GetSelectNodes(L"root/Area");
			long AreaCountNode = XmlDocReader::GetLength(Nodes);
			for (long i = 0; i < AreaCountNode; i++) {
				auto AreaNode = XmlDocReader::GetItem(Nodes, i);
				auto AreaNumStr = XmlDocReader::GetAttribute(AreaNode, L"AreaNumber");
				int AreaNum = (int)_wtoi(AreaNumStr.c_str());
				if (AreaNum != SelectArea)
					continue;
				//子要素を取得
				auto StageNodes = XmlDocReader::GetChildNodes(AreaNode);
				long StageCountNode = XmlDocReader::GetLength(StageNodes);
				for (long j = 0; j < StageCountNode; j++)
				{
					auto StageNode = XmlDocReader::GetItem(StageNodes, j);
					auto StageNumStr = XmlDocReader::GetAttribute(StageNode, L"StageNumber");
					int StageNum = (int)_wtoi(StageNumStr.c_str());
					if (StageNum != SelectStage)
						continue;
					//カメラ情報取得
					//auto CameraEyeStr = XmlDocReader::GetAttribute(StageNodes, L"CameraEye");
					//auto CameraAtStr = XmlDocReader::GetAttribute(StageNodes, L"CameraAt");
					//auto CameraNearStr = XmlDocReader::GetAttribute(StageNodes, L"CamereNear");
					//auto CameraFarStr = XmlDocReader::GetAttribute(StageNodes, L"CamereFar");

					////トークン
					//if (CameraEyeStr != L"") 
					//{
					//	vector<wstring> Token;
					//	Token.clear();
					//	Util::WStrToTokenVector(Token, CameraEyeStr, L',');
					//	Eye.x = (float)_wtof(Token[0].c_str());
					//	Eye.y = (float)_wtof(Token[1].c_str());
					//	Eye.z = (float)_wtof(Token[2].c_str());

					//	Token.clear();
					//	Util::WStrToTokenVector(Token, CameraAtStr, L',');
					//	At.x = (float)_wtof(Token[0].c_str());
					//	At.y = (float)_wtof(Token[1].c_str());
					//	At.z = (float)_wtof(Token[2].c_str());

					//	Near = (float)_wtof(CameraNearStr.c_str());
					//	Far = (float)_wtof(CameraFarStr.c_str());
					//}
					//子要素を取得
					auto ObjNodes = XmlDocReader::GetChildNodes(StageNode);
					long ObjCount = XmlDocReader::GetLength(ObjNodes);
					for (long k = 0; k < ObjCount; k++)
					{
						auto ObjNode = XmlDocReader::GetItem(ObjNodes, k);
						auto TypeStr = XmlDocReader::GetAttribute(ObjNode, L"Type");
						CreateFromXML(TypeStr, StagePtr, ObjNode);
					}
				}
			}

			//Area
			//カメラ情報を更新する
			auto MainView = StagePtr->GetMainView();

			auto Camera = MainView->GetCamera();
			Camera->SetEye(Eye);
			Camera->SetAt(At);
			Camera->SetNear(Near);
			Camera->SetFar(Far);

		}
		catch (...) {
			throw;
		}
	}

}