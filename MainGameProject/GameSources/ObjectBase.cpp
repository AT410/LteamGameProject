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
	}
}