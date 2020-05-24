/*!
@breif	�v���W�F�N�g��`�̒ǉ��֐��E�@�\
@name	�쐬�ҁF�����B��
*/

#include "stdafx.h"
#include "Project.h"

#define WSTR(var) #var

namespace basecross
{
	//--------------------------------------------------------------------------------------
//	struct StageBulider::Impl;
//	�p�r: Impl�C�f�B�I��
//--------------------------------------------------------------------------------------
	struct StageBulider::Impl {
		map<wstring, shared_ptr<GameObjectCreatorBaseXML> > m_CreatorMap;

		Impl()
		{
		}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	�X�e�[�W�r���_�[ToXML
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
			//�J�����ݒ�l
			Vec3 Eye, At;
			float Near, Far;

			Eye = Vec3(7.0f, 10.0f, -20.0f);
			At = Vec3(7.0f, 2.0f, 0.0f);
			Near = 0.5f;
			Far = 1000.0f;

			//�X�e�[�W�I��l���擾
			auto Select = GameManager::GetManager()->GetStagePair();
			int SelectArea = Select.first;
			int SelectStage = Select.second;
			//XML���[�_�[
			XmlDocReader Reader(XMLFileName);
			auto Nodes = Reader.GetSelectNodes(L"root/Area");
			long AreaCountNode = XmlDocReader::GetLength(Nodes);
			for (long i = 0; i < AreaCountNode; i++) {
				auto AreaNode = XmlDocReader::GetItem(Nodes, i);
				auto AreaNumStr = XmlDocReader::GetAttribute(AreaNode, L"AreaNumber");
				int AreaNum = (int)_wtoi(AreaNumStr.c_str());
				if (AreaNum != SelectArea)
					continue;
				//�q�v�f���擾
				auto StageNodes = XmlDocReader::GetChildNodes(AreaNode);
				long StageCountNode = XmlDocReader::GetLength(StageNodes);
				for (long j = 0; j < StageCountNode; j++)
				{
					auto StageNode = XmlDocReader::GetItem(StageNodes, j);
					auto StageNumStr = XmlDocReader::GetAttribute(StageNode, L"StageNumber");
					int StageNum = (int)_wtoi(StageNumStr.c_str());
					if (StageNum != SelectStage)
						continue;
					//�J�������擾
					auto CameraEyeStr = XmlDocReader::GetAttribute(StageNode, L"CameraEye");
					auto CameraAtStr = XmlDocReader::GetAttribute(StageNode, L"CameraAt");
					auto CameraNearStr = XmlDocReader::GetAttribute(StageNode, L"CameraNear");
					auto CameraFarStr = XmlDocReader::GetAttribute(StageNode, L"CameraFar");

					//�g�[�N��
					if (CameraEyeStr != L"") 
					{
						vector<wstring> Token;
						Token.clear();
						Util::WStrToTokenVector(Token, CameraEyeStr, L',');
						Eye.x = (float)_wtof(Token[0].c_str());
						Eye.y = (float)_wtof(Token[1].c_str());
						Eye.z = (float)_wtof(Token[2].c_str());

						Token.clear();
						Util::WStrToTokenVector(Token, CameraAtStr, L',');
						At.x = (float)_wtof(Token[0].c_str());
						At.y = (float)_wtof(Token[1].c_str());
						At.z = (float)_wtof(Token[2].c_str());

						Near = (float)_wtof(CameraNearStr.c_str());
						Far = (float)_wtof(CameraFarStr.c_str());
					}
					//�q�v�f���擾
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
			//�J���������X�V����
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

	void StageBulider::UISetBuild(const shared_ptr<StageBase>&StagePtr, const wstring& XMLFile, const bool DefaultDrawActive)
	{
		//XML���[�_�[
		XmlDocReader Reader(XMLFile);
		auto Nodes = Reader.GetSelectNodes(L"UIRoot/UISet");
		long UISetCount = XmlDocReader::GetLength(Nodes);
		for (long i = 0; i < UISetCount; i++) {
			auto UITypeNode = XmlDocReader::GetItem(Nodes, i);
			auto UITypeStr = XmlDocReader::GetAttribute(UITypeNode, L"UIType");
			auto SType = StagePtr->GetStageTypeStr();
			if (UITypeStr != SType)
				continue;
			//�q�v�f���擾
			auto UIDataNodes = XmlDocReader::GetChildNodes(UITypeNode);
			long UIDataCount = XmlDocReader::GetLength(UIDataNodes);
			for (long j = 0; j < UIDataCount; j++)
			{
				auto UIDataNode = XmlDocReader::GetItem(UIDataNodes, j);
				auto TypeStr = XmlDocReader::GetAttribute(UIDataNode, L"Type");
				//�q�v�f���擾
				auto Ptr = CreateFromXML(TypeStr, StagePtr, UIDataNode);
				//Ptr->SetDrawActive(DefaultDrawActive);
			}
		}
	}

}