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
		//�v���C���[�̃W�����v���f
		bool m_Jumpjudge;
		//�v���C���[�̓����~���f
		bool m_StopActionTimeJudge;
		//�����o���������f
		bool m_PushPull;
		//�v���C���[�X�e�[�g�擾�ϐ�
		PlayerState m_PlayerState;

		shared_ptr<GameObject> m_PushObj = nullptr;
		//���̓n���h���[
		InputHandler<Player> m_Handler;
		Vec3 GetMoveVector() const;

	public:
		Player(const shared_ptr<Stage>& Stageptr);

		Player(const shared_ptr<Stage>& StagePtr, const Vec3 Position, const Vec3 Rotation, const Vec3 Scale,
			const wstring TexKey, const wstring MeshKey)
			:ObjectBase(StagePtr, Position, Rotation, Scale, TexKey, MeshKey), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false)

		{
		}

		Player(const shared_ptr<Stage>&StagePtr, IXMLDOMNodePtr pNode)
			:ObjectBase(StagePtr, pNode), m_Speed(3.0f),
			m_Jumpforce(4.0f),
			m_StopActionTime(5.0f),
			m_Jumpjudge(false),
			m_StopActionTimeJudge(false)
		{}

		virtual ~Player() {}
		//�v���C���[�ړ��֐�
		void Move();
		//ObjectBase�p��->A�{�^�����̓W�����v�֐�
		void OnPushA() override;
		//objectBase�p��->LB�{�^�����͕���������֐�
		void OnPushLB() override;
		void OnRemoveLB() override;
		//�v���C���[�X�e�[�g�ω��֐�
		void State();
		//�v���C���[�X�^�[�g�X�e�[�g
		void StartState();
		//�v���C���[�S�[���X�e�[�g	
		void ClearState();
		void OnCreate() override;
		void OnUpdate() override;
		void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		void OnCollisionExit(shared_ptr<GameObject>& Obj)override;
	
		void ResetPositon()
		{
			GetComponent<Transform>()->SetPosition(m_pos);
		}
	};


}
//end basecross

