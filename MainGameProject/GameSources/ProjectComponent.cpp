/*!
@file ProjectBehavior.cpp
@brief プロジェク定義の行動クラス実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct ActionComponent::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct AnimationComponent::Impl {
		bool m_Run;			//移動中かどうか
		bool m_Arrived;	//到着したかどうか
		float m_TotalTime;	//移動にかける時間
		float m_NowTime;	//現在の時間
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
	//	用途: アクション系コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	AnimationComponent::AnimationComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	AnimationComponent::~AnimationComponent() {}

	//アクセサ
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
			//到着してなければ動作させる
			pImpl->m_Run = true;
		}
	}

	//NowTimeを加算して、TotalTimeと比較する
	bool AnimationComponent::AdditionalNowTime() {
		if (pImpl->m_Run) {
			if (pImpl->m_TotalTime > 0) {
				float ElapsedTime = App::GetApp()->GetElapsedTime();
				pImpl->m_NowTime += ElapsedTime;
				if (pImpl->m_NowTime >= pImpl->m_TotalTime) {
					pImpl->m_NowTime = pImpl->m_TotalTime;
					pImpl->m_Run = false;
					//到着した
					pImpl->m_Arrived = true;
				}
				return true;
			}
		}
		return false;
	}
}

//end basecross

