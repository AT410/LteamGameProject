/*!
@file Player.h
@brief �v���C���[�Ȃ�
@name �쐬�ҁF���蟩���iPlaerState enum�N���X, Player�N���X�j
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
		const int m_cntlNum;
		//�v���C���[�̏ꏊ
		Vec3 m_Position;
		//�v���C���[�̊p�x
		Vec3 m_Rotation;
		//�v���C���[�̑傫��
		Vec3 m_Scale;
		//�v���C���[�̐i�s�����̌���
		Vec3 m_PlayerAngle;
		Vec3 GetMoveVector() const;
		//�R���g���[���[����擾
		Controller m_cntl;
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
		InputHandler<Player> m_inputHandler;
	public:
		Player(const shared_ptr<Stage>& Stageptr);
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
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Obj) override;
		virtual void OnCollisionExit(shared_ptr<GameObject>& Obj)override;
	};

}
//end basecross

