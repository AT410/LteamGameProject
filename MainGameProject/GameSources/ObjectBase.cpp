/*!
@breif	�I�u�W�F�N�g�̊��N���X����
@name	�쐬�ҁF�����B��
*/
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	ObjectBase::ObjectBase(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
		:GameObject(StagePtr)
	{
		//�z�u�f�[�^�A�e�N�X�`���L�[�A���b�V���L�[
		auto MeshStr = XmlDocReader::GetAttribute(pNode, L"MeshKey");
		auto TexStr = XmlDocReader::GetAttribute(pNode, L"TexKey");
		auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto ScaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
		auto RotStr = XmlDocReader::GetAttribute(pNode, L"Rot");
		auto TagStr = XmlDocReader::GetAttribute(pNode, L"Tags");

		//���b�V��
		m_meshKey = MeshStr;

		//�e�N�X�`��
		m_texKey = TexStr;

		//�g�[�N���i�J�����j�̔z��
		vector<wstring> Tokens;
		//�g�[�N���i�J�����j�P�ʂŕ�����𒊏o(L','���f���~�^�Ƃ��ċ敪��)
		//Position
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, PosStr, L',');
		//�e�g�[�N���i�J�����j���X�P�[���A��]�A�ʒu�ɓǂݍ���
		m_pos = Vec3(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		//Scale
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, ScaleStr, L',');
		m_scal = Vec3(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		//Rot
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, RotStr, L',');
		//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
		m_rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
		m_rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
		m_rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
		m_rot.w = (Tokens[3] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[3].c_str());

		//�^�O����ǂݍ���
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, TagStr, L',');

		m_tag = Tokens;

		// -- �����ݒ� --
		auto CollsionActiveStr = XmlDocReader::GetAttribute(pNode, L"CollisionActive");
		m_CollisionActive = (bool)_wtoi(CollsionActiveStr.c_str());
		if (m_CollisionActive)
		{
			auto CollsionFixedStr = XmlDocReader::GetAttribute(pNode, L"CollisionFixedActive");
			m_CollisionSetFixed = (bool)_wtoi(CollsionFixedStr.c_str());
		}

		//���L�ݒ�
		auto SharedFStr = XmlDocReader::GetAttribute(pNode, L"SharedActive");
		m_SharedActive = (bool)_wtoi(SharedFStr.c_str());

		if (m_SharedActive)
		{
			m_SharedName = XmlDocReader::GetAttribute(pNode, L"SharedKey");
		}

		// -- �C�x���g�ݒ� --
		auto EventFStr = XmlDocReader::GetAttribute(pNode, L"EventActive");
		m_EventActive = (bool)_wtoi(EventFStr.c_str());

		if (m_EventActive)
		{
			m_ReceiverKey = XmlDocReader::GetAttribute(pNode, L"EventReceiverKey");
		}

		// -- �A�j���[�V�����ݒ� --
		m_AnimationNodes = XmlDocReader::GetChildNodes(pNode);
		long AnimationsCount = XmlDocReader::GetLength(m_AnimationNodes);
		if (AnimationsCount != 0)
		{
			m_AnimationActive = true;
		}
	}

	void ObjectBase::OnEvent(const shared_ptr<Event>&event)
	{
		if (m_AnimationActive) 
		{
			if (event->m_MsgStr == L"StartAction")
			{
				if (!m_StartActionActive)
					return;
				GetComponent<Actions>()->Run(L"Start");
			}
			else if (event->m_MsgStr == L"EndAction")
			{
				if (!m_EndActionActive)
					return;
				GetComponent<Actions>()->Run(L"End");
			}
		}
	}

	void ObjectBase::DefaultSettings()
	{
		// -- �`��ݒ� --
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey,false);
		
		// -- �z�u�ݒ� --
		auto TransComp = GetComponent<Transform>();
		TransComp->SetScale(m_scal);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetPosition(m_pos);

		// -- �����ݒ� --
		if (m_CollisionActive)
		{
			auto CollComp = AddComponent<CollisionObb>();
			CollComp->SetFixed(m_CollisionSetFixed);
		}

		// -- �^�O�ݒ� --
		for (auto tag : m_tag)
		{
			if (tag == L"")
				continue;
			AddTag(tag);
		}

		// -- ���L�ݒ� --
		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<ObjectBase>());
		}

		// -- �C�x���g�ݒ� --
		if (m_EventActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(m_ReceiverKey, GetThis<ObjectInterface>());
		}

		if (m_AnimationActive)
		{
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(L"Start", GetThis<ObjectInterface>());
			App::GetApp()->GetEventDispatcher()->AddEventReceiverGroup(L"End", GetThis<ObjectInterface>());
		}
	}

	void ObjectBase::SetActions()
	{
		if (m_AnimationActive) {
			auto ActionPtr = AddComponent<Actions>();
			long AnimationsCount = XmlDocReader::GetLength(m_AnimationNodes);
			for (long a = 0; a < AnimationsCount; a++)
			{
				auto AnimationNode = XmlDocReader::GetItem(m_AnimationNodes, a);
				wstring AnimationKey = XmlDocReader::GetAttribute(AnimationNode, L"CallPoint");
				auto LoopFlagStr = XmlDocReader::GetAttribute(AnimationNode, L"LoopActive");

				if (AnimationKey == L"Start") 
				{
					m_StartActionActive = true;
					m_IsStartActionLoop = (bool)_wtoi(LoopFlagStr.c_str());
				}
				else if (AnimationKey == L"OnEvent")
				{
					m_EventActionActive = true;
					m_IsEventActionLoop = true;
				}
				else if (AnimationKey == L"End")
				{
					m_EndActionActive = true;
					m_IsEndActionLoop = true;
				}

				// -- �A�j���[�V�����f�[�^ --
				auto AnimationDatas = XmlDocReader::GetChildNodes(AnimationNode);
				long DataCount = XmlDocReader::GetLength(AnimationDatas);
				vector<wstring> Tokens;
				for (long l = 0; l < DataCount; l++)
				{
					auto AnimationData = XmlDocReader::GetItem(AnimationDatas, l);
					wstring FlameStr = XmlDocReader::GetAttribute(AnimationData, L"FlameCount");
					wstring TypeStr = XmlDocReader::GetAttribute(AnimationData, L"Type");
					wstring ValueStr = XmlDocReader::GetAttribute(AnimationData, L"Value");

					float FlameCount = (float)_wtof(FlameStr.c_str());
					if (TypeStr == L"Postion")
					{
						Tokens.clear();
						Util::WStrToTokenVector(Tokens, ValueStr, L',');
						Vec3 Pos = Vec3(
							(float)_wtof(Tokens[0].c_str()),
							(float)_wtof(Tokens[1].c_str()),
							(float)_wtof(Tokens[2].c_str())
						);
						ActionPtr->AddMoveTo(AnimationKey, FlameCount, Pos);
					}
					else if (TypeStr == L"Rotation")
					{
						//Rot
						Tokens.clear();
						Util::WStrToTokenVector(Tokens, ValueStr, L',');
						//��]�́uXM_PIDIV2�v�̕�����ɂȂ��Ă���ꍇ������
						Vec4 Rot;
						Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
						Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
						Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
						Rot.w = (Tokens[3] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[3].c_str());

						ActionPtr->AddRotateTo(AnimationKey, FlameCount, Quat(Rot));
					}
					else if (TypeStr == L"Scale")
					{
						Tokens.clear();
						Util::WStrToTokenVector(Tokens, ValueStr, L',');
						Vec3 Scale = Vec3(
							(float)_wtof(Tokens[0].c_str()),
							(float)_wtof(Tokens[1].c_str()),
							(float)_wtof(Tokens[2].c_str())
						);

						ActionPtr->AddScaleTo(AnimationKey, FlameCount, Scale);
					}
				}
			}
		}
	}
}