/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//---------------------------------------------------
	//�v���C���[�X�e�[�g
	//@�쐬�ҁF���蟩��
	//---------------------------------------------------
	enum class PlayerState {
		Start,
		Excute,
		Clear,
		Restart,
	};
	//---------------------------------------------------
	//�v���C���[�N���X
	//@�쐬�ҁF���蟩��
	//---------------------------------------------------
	class Player : public ObjectBase, public PawnBase<Player> {
	private:
		//�v���C���[�̐i�s�����̌���
		Vec3 m_PlayerAngle;

		//�v���C���[�̑���
		float m_Speed;
		//�v���C���[�̃W�����v�̋���
		float m_Jumpforce;
		//�v���C���[�̓����~����
		float m_StopActionTime;
		//�v���C���[�ƃI�u�W�F�N�g�̋���
		float m_distance;

		float m_JumpPos;

		float m_RisePos;
		//�v���C���[�̃W�����v���f
		bool m_Jumpjudge;
		//�v���C���[�̓����~���f
		bool m_StopActionTimeJudge;
		//�����o���������f
		bool m_PushPull;
		//�����o�������o���I�u�W�F�N�g���f
		bool m_PushBoxActiv;
		//�v���C���[�X�e�[�g�擾�ϐ�
		PlayerState m_PlayerState;

		shared_ptr<GameObject> m_PushObj = nullptr;
		//���̓n���h���[
		InputHandler<Player> m_Handler;
		Vec3 GetMoveVector() const;

		shared_ptr<EfkPlay> m_FireEfk;

		shared_ptr<SoundItem> m_ClearSound;

	public:
		Player(const shared_ptr<Stage>& Stageptr);

		Player(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false),
			m_PushBoxActiv(false)

		{
		}

		Player(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false),
			m_PushBoxActiv(false)
		{}

		virtual ~Player() {}
		//�v���C���[�ړ��֐�
		void Move();
		//ObjectBase�p��->A�{�^�����̓W�����v�֐�
		void OnPushA() override;
		//objectBase�p��->LB�{�^�����͕���������֐�
		void OnPushLB() override;
		//objectBase�p��->LB�{�^��������������������֐�
		void OnRemoveLB() override;
		//���̈������艟�����݉���
		void PushPullRelese();
		void StopPhysics();
		//�v���C���[�X�e�[�g�ω��֐�
		void StateUpdate();
		//�v���C���[�X�^�[�g�X�e�[�g
		void StartBehavior();
		
		void ExcuteBehavior();
		//�v���C���[�S�[���X�e�[�g	
		void ClearBehavior();
		void RestartBehabior();
		//
		bool GetPushBoxActiv() { return m_PushBoxActiv; };
		void SetState(PlayerState State) { m_PlayerState = State; }
		void OnCreate() override;
		void OnUpdate() override;
		void OnEvent(const shared_ptr<Event>& Eve) override;
		void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExit(shared_ptr<GameObject>& Obj)override;
	
		void ResetPositon()
		{
			GetComponent<Transform>()->SetPosition(m_pos);
			PushPullRelese();
			auto EfkPoint = m_pos;
			EfkPoint.y += 0.5f;
			m_FireEfk = ObjectFactory::Create<EfkPlay>(L"FIRE_EFK", EfkPoint);
		}

		void UpdateResetPositon()
		{
			m_pos = GetComponent<Transform>()->GetPosition();
		}
	};


}
//end basecross

