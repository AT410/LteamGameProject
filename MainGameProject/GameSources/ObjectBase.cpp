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
		auto MeshStr = XmlDocReader::GetAttribute(pNode, L"MeshKey");
		auto TexStr = XmlDocReader::GetAttribute(pNode, L"TexKey");
		auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto ScaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
		auto RotStr = XmlDocReader::GetAttribute(pNode, L"Rot");
		auto TagStr = XmlDocReader::GetAttribute(pNode, L"Tags");

		//メッシュ
		m_meshKey = MeshStr;

		//テクスチャ
		m_texKey = TexStr;

		//トークン（カラム）の配列
		vector<wstring> Tokens;
		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
		//Position
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, PosStr, L',');
		//各トークン（カラム）をスケール、回転、位置に読み込む
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
		//回転は「XM_PIDIV2」の文字列になっている場合がある
		m_rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
		m_rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
		m_rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
		m_rot.w = (Tokens[3] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[3].c_str());

		//タグ情報を読み込む
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, TagStr, L',');

		m_tag = Tokens;

		// -- 物理設定 --
		auto CollsionActiveStr = XmlDocReader::GetAttribute(pNode, L"CollisionActive");
		m_CollisionActive = (bool)_wtoi(CollsionActiveStr.c_str());
		if (m_CollisionActive)
		{
			auto CollsionFixedStr = XmlDocReader::GetAttribute(pNode, L"CollisionFixedActive");
			m_CollisionSetFixed = (bool)_wtoi(CollsionFixedStr.c_str());
		}

		//共有設定
		auto SharedFStr = XmlDocReader::GetAttribute(pNode, L"SharedActive");
		m_SharedActive = (bool)_wtoi(SharedFStr.c_str());

		if (m_SharedActive)
		{
			m_SharedName = XmlDocReader::GetAttribute(pNode, L"SharedKey");
		}

		// -- イベント設定 --
		auto EventFStr = XmlDocReader::GetAttribute(pNode, L"EventActive");
		m_EventActive = (bool)_wtoi(EventFStr.c_str());

		if (m_EventActive)
		{
			m_ReceiverKey = XmlDocReader::GetAttribute(pNode, L"EventReceiverKey");
		}

		// -- アニメーション設定 --
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
		// -- 描画設定 --
		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetMeshResource(m_meshKey);
		DrawComp->SetTextureResource(m_texKey,false);
		
		// -- 配置設定 --
		auto TransComp = GetComponent<Transform>();
		TransComp->SetScale(m_scal);
		TransComp->SetQuaternion(Quat(m_rot));
		TransComp->SetPosition(m_pos);

		// -- 物理設定 --
		if (m_CollisionActive)
		{
			auto CollComp = AddComponent<CollisionObb>();
			CollComp->SetFixed(m_CollisionSetFixed);
		}

		// -- タグ設定 --
		for (auto tag : m_tag)
		{
			if (tag == L"")
				continue;
			AddTag(tag);
		}

		// -- 共有設定 --
		if (m_SharedActive)
		{
			GetStage()->SetSharedGameObject(m_SharedName, GetThis<ObjectBase>());
		}

		// -- イベント設定 --
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

				// -- アニメーションデータ --
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
						//回転は「XM_PIDIV2」の文字列になっている場合がある
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