//@breif �����˃M�~�b�N�p
//@name ����@����
#pragma once
#include"stdafx.h"

namespace basecross {
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

	public:
		WaterJet(const shared_ptr<Stage>& Stageptr);
		virtual ~WaterJet() {}
		void SetSizeAABBX(const float sizeX);
		void SetSizeAABBY(const float sizeY);
		void SetSizeAABBZ(const float sizeZ);
		void WaterJetJudgment();
		void StartJudgment();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};



}