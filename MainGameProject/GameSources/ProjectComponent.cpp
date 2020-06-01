/*!
@file ProjectBehavior.cpp
@brief �v���W�F�N��`�̍s���N���X����
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct AnimationComponent::Impl {
		bool m_Run;			//�ړ������ǂ���
		bool m_Arrived;	//�����������ǂ���
		float m_TotalTime;	//�ړ��ɂ����鎞��
		float m_NowTime;	//���݂̎���
		Impl() :
			m_Run(false),
			m_Arrived(false),
			m_TotalTime(0),
			m_NowTime(0)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ActionComponent : public Component ;
	//	�p�r: �A�N�V�����n�R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	AnimationComponent::AnimationComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	AnimationComponent::~AnimationComponent() {}

	//�A�N�Z�T
	void AnimationComponent::SetRun(bool b) {
		pImpl->m_Run = b;
	}
	void  AnimationComponent::SetArrived(bool b) {
		pImpl->m_Arrived = b;
	}


	bool AnimationComponent::IsRun()const {
		return pImpl->m_Run;
	}
	bool AnimationComponent::GetRun()const {
		return pImpl->m_Run;
	}

	bool AnimationComponent::IsArrived()const {
		return pImpl->m_Arrived;
	}
	bool AnimationComponent::GetArrived()const {
		return pImpl->m_Arrived;
	}

	float AnimationComponent::GetNowTime() const {
		return pImpl->m_NowTime;
	}
	void AnimationComponent::SetNowTime(float f) {
		pImpl->m_NowTime = f;
	}

	void AnimationComponent::Stop() {
		pImpl->m_Run = false;
	}
	void AnimationComponent::ReStart() {
		if (!pImpl->m_Arrived) {
			//�������ĂȂ���Γ��삳����
			pImpl->m_Run = true;
		}
	}

	//NowTime�����Z���āATotalTime�Ɣ�r����
	bool AnimationComponent::AdditionalNowTime() {
		if (pImpl->m_Run) {
			if (pImpl->m_TotalTime > 0) {
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime) {
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//��������
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}
}

//end basecross

