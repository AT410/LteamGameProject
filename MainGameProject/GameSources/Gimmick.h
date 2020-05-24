/*!
@breif	Gimmick�֘A
@name	�쐬�ҁF�����B��
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	///<breif>�X�C�b�`�I�u�W�F�N�g<breif/>
	///<name>�쐬�ҁF�����B��<name/>
	class SwitchObj : public ObjectBase
	{
	public:
		SwitchObj(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey,const bool IsKeep = false)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_IsKeep(IsKeep), m_Active(false)
		{
		}

		SwitchObj(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		void OnCreate() override;
		void OnUpdate()override
		{
			auto transComp = GetComponent<Transform>();
			transComp->SetPosition(m_pos);
		}

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		void OnCollisionExit(shared_ptr<GameObject>& Other)	override;

		//�Q�b�^�[
		bool GetSwitchActiveI()const { return m_Active; }
	private:
		bool m_Active;//ON�EOFF����
		//����������K�v�����邩�B
		bool m_IsKeep;

		//�C�x���g�ݒ�
		wstring m_RecipientKey;//<-��M��ݒ�L�[
		wstring m_EventMsg; //<-�C�x���g���b�Z�[�W

		//�G�t�F�N�g
		shared_ptr<EfkPlay> m_ActiveEfk;
	};

	//----------------------------------------------------------------------------
	//���ΐ�
	//----------------------------------------------------------------------------
	class FireLine : public ObjectBase
	{
	public:
		//-- �\�z --
		FireLine(const shared_ptr<Stage>& StagePtr)
			:ObjectBase(StagePtr)
		{
		}

		FireLine(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);


		//-- �j�� --
		virtual ~FireLine() {}

		//-- ������ --
		void OnCreate()override;

		//-- �X�V���� --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>&event);

	private:
		float m_Time = 0;
		bool m_Active = false;

		//�C�x���g�ݒ�
		wstring m_RecipientKey;//<-��M��ݒ�L�[
		wstring m_EventMsg; //<-�C�x���g���b�Z�[�W
	};

	///<breif>�M��`����_<breif/>
	///<name> �쐬�ҁF�ɓ��ˌ�<name/>
	class HeatStick : public ObjectBase
	{
	private:
		AABB m_HeatArea;

		shared_ptr<Player> m_player;
		shared_ptr<ObjectBase> m_himo;

		//�f�B���C�J�E���g
		float m_count;
	public:
		HeatStick(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_count(0)
		{
		}
		HeatStick(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;
	};

	//������
	class Omori : public ObjectBase
	{
	public:
		Omori(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Omori(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
	};

	//������
	class MoveFloor : public ObjectBase
	{
	private:
		float m_Time;
		bool m_Active; //flg
	public:
		MoveFloor(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		MoveFloor(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}


		void OnCreate() override;
		void OnUpdate() override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};
	//-----------------------------------------------------------------------------
	//���̃I�u�W�F�N�g
	//-----------------------------------------------------------------------------
	class Door :public ObjectBase
	{
	public:
		Door(const shared_ptr<Stage>&StagePtr)
			:ObjectBase(StagePtr){}

		// -- Xml�t�@�C���Ǎ��p --
		Door(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode);

		virtual ~Door(){}

		// -- ������ --
		void OnCreate()override;

		// -- �X�V���� --
		void OnUpdate()override;

		void OnEvent(const shared_ptr<Event>& event)override;

		// -- �Z�b�^�[ --
		void SetOpenActive(bool Active) { m_OpenActive = Active; }

	private:
		// -- �J�又�� --
		void OpenProcess();
		// -- �又�� --
		void CloseProcess();

		bool MoveBehavior(Vec3 Start,Vec3 End, const float TotalTime);

		float m_TotalTime;
		bool m_OpenActive;
		bool m_MoveEnd;
	};

	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	class Fountain :public ObjectBase
	{
	private:
		bool m_Active; //flg
		float m_Time = 0;

		AABB m_FountainArea; //��ɗL��
		AABB m_FountainArea2; //����Active�̎������ɗL��

		shared_ptr<Player> m_TestPlayer;

		shared_ptr<EfkPlay> m_WaterEfk;

	public:
		Fountain(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		Fountain(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode) {}

		void OnCreate() override;
		void OnUpdate() override;

		void OnCollisionEnter(shared_ptr<GameObject>& Other) override;

		void SetActive(bool active)
		{
			m_Active = active;
		}
	};


	//-----------------------------------------------------------------------------
	//��N���X
	//-----------------------------------------------------------------------------
	class Waterfall :public ObjectBase
	{
	public:
		Waterfall(const shared_ptr<Stage>&StagePtr,const Vec3& Start,const Vec3& End,const float Width,const float Speed)
			:ObjectBase(StagePtr),m_StartPoint(Start),m_EndPoint(End),m_Width(Width),m_FallSpeed(Speed){}

		Waterfall(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey)
		{
		}

		void OnCreate()override;

		void OnUpdate()override;

	private:
		Vec3 m_StartPoint;
		Vec3 m_EndPoint;

		float m_Width;
		float m_FallSpeed;

		float m_TotalTime = 0;

		shared_ptr<EfkPlay> m_EfkPlay;
	};

	//�E�H�[�^�[�W�F�b�g�N���X
	//������ҁ@���蟩��
	class WaterJet : public ObjectBase {
		Vec3 m_Pos;
		Vec3 m_StartPos;
		bool m_WaterJetmode; //�����˂�ON/OFF�X�C�b�`
		bool m_WaterJetDirToUp; //�����˂̌����@true������� false��������
		AABB m_WaterJetAABB;
		float m_JudmentTime;
		float m_SizeAABBX;
		float m_SizeAABBY;
		float m_SizeAABBZ;

		shared_ptr<EfkPlay> m_efk;
		//�C�x���g�ݒ�
		wstring m_RecipientKey;//<-��M��ݒ�L�[
		wstring m_EventMsg; //<-�C�x���g���b�Z�[�W


	public:
		WaterJet(const shared_ptr<Stage>& Stageptr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			: ObjectBase(Stageptr, Position, Rotation, Scale, TexKey, MeshKey),	m_WaterJetmode(false),m_WaterJetDirToUp(false),
			m_SizeAABBX(2.0f),m_SizeAABBY(3.0f),m_SizeAABBZ(2.0f)
		{}
		WaterJet(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterJet() {}
		void SetSizeAABBX(const float sizeX);
		void SetSizeAABBY(const float sizeY);
		void SetSizeAABBZ(const float sizeZ);
		void WaterJetJudgment();
		void StartJudgment();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};


	class WaterDrop : public ObjectBase {
		float m_Speed;
		float m_FallSpeed;
		float m_time;
		float m_SpeedAdd;
		float m_WaterAABBX;
		float m_WaterAABBY;
		float m_WaterAABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		AABB m_WaterDropAABB;
	public:
		WaterDrop(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(0.0f),m_FallSpeed(0.0f),m_time(3.0f),m_SpeedAdd(0.4f)
		{}
		WaterDrop(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterDrop() {}
		void Drop();
		void WaterDropJudgement();
		virtual void OnCreate();
		virtual void OnUpdate();

		void OnCollisionEnter(shared_ptr<GameObject>&Obj)override;

	};

	//�E�H�[�^�[���x���N���X
//������ҁ@���蟩��
	class WaterLV : public ObjectBase {
		float m_Waterheight;
		float m_WaterTime;
		float m_AABBX;
		float m_AABBY;
		float m_AABBZ;
		float m_PlayerAABBX;
		float m_PlayerAABBY;
		float m_PlayerAABBZ;
		bool m_WaterLVMode;
		Vec3 m_WaterOldPos;
		Vec3 m_WaterCurrentPos;
		AABB m_WaterLVAABB;
		AABB m_PlayerAABB;
	public:
		WaterLV(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey) 
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey),m_WaterLVMode(false),m_AABBX(1.0f),m_AABBY(1.0f),m_AABBZ(1.0f) {}
		WaterLV(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~WaterLV() {}
		void SetWaterLVMode(const bool LVMode);
		void WaterLVChange();
		void WaterLVJudgement();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		void OnEvent(const shared_ptr<Event>&event);
	};


	class UpDownBox : public ObjectBase {
		float m_Speed;
		float m_totaltime;
		float m_parenttime;
		AABB m_BoxAABB;
		Vec3 m_OldPos;
		Vec3 m_CurrentPos;
		bool m_ParentJudge;
		bool FloatMove();
	public:
		UpDownBox(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(1.0f), m_OldPos(0.0f), m_parenttime(2.0f){}
		UpDownBox(const shared_ptr<Stage>& Stageptr, IXMLDOMNodePtr pNode);
		virtual ~UpDownBox() {}
		void BoxJudgment();
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj) override;
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};

}