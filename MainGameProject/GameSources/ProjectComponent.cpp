/*!
@file ProjectBehavior.cpp
@brief プロジェク定義の行動クラス実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	struct Action::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Actions::Impl {
		bool m_Looped;

		bool m_ScaleArrived;	//m_Loopがfalseのときのみ有効
		bool m_RotateArrived;	//m_Loopがfalseのときのみ有効
		bool m_MoveArrived;	//m_Loopがfalseのときのみ有効


		map<wstring,vector<shared_ptr<ActionComponent> >> m_ScaleVec;
		map<wstring, vector<shared_ptr<ActionComponent> >> m_RotateVec;
		map<wstring, vector<shared_ptr<ActionComponent> >> m_MoveVec;

		wstring m_CurrntMapKey;

		size_t m_ScaleActiveIndex;
		size_t m_RotateActiveIndex;
		size_t m_MoveActiveIndex;

		Impl() :
			m_Looped(false),
			m_ScaleArrived(false),
			m_RotateArrived(false),
			m_MoveArrived(false),
			m_ScaleActiveIndex(0),
			m_RotateActiveIndex(0),
			m_MoveActiveIndex(0)
		{}
		~Impl() {}

		void SetCurrntActionKey(const wstring ActionKey)
		{
			auto Moveit = m_MoveVec.find(ActionKey);
			auto Rotit = m_RotateVec.find(ActionKey);
			auto Scaleit = m_ScaleVec.find(ActionKey);
			if (Moveit == m_MoveVec.end() && Rotit == m_RotateVec.end() && Scaleit == m_ScaleVec.end())
			{
				throw(BaseException(L"該当アクションが登録されていません。",
					L"Actions::Impl::SetCurrntActionKey",
					L"Map To NotFound!!"));
			}
			m_CurrntMapKey = ActionKey;
		}

		void AddScaleVec(wstring ActionKey, shared_ptr<ActionComponent> Action)
		{
			auto it = m_ScaleVec.find(ActionKey);
			if (it != m_ScaleVec.end()) {
				//キーがあった
				it->second.push_back(Action);
			}
			else {
				//グループがない
				vector<shared_ptr<ActionComponent>> vec;
				m_ScaleVec[ActionKey] = vec;
				m_ScaleVec[ActionKey].push_back(Action);
			}
		}

		void AddMoveVec(wstring ActionKey, shared_ptr<ActionComponent> Action)
		{
			auto it = m_MoveVec.find(ActionKey);
			if (it != m_MoveVec.end()) {
				//キーがあった
				it->second.push_back(Action);
			}
			else {
				//グループがない
				vector<shared_ptr<ActionComponent>> vec;
				m_MoveVec[ActionKey] = vec;
				m_MoveVec[ActionKey].push_back(Action);
			}
		}
		void AddRotateVec(wstring ActionKey, shared_ptr<ActionComponent> Action)
		{
			auto it = m_RotateVec.find(ActionKey);
			if (it != m_RotateVec.end()) {
				//キーがあった
				it->second.push_back(Action);
			}
			else {
				//グループがない
				vector<shared_ptr<ActionComponent>> vec;
				m_RotateVec[ActionKey] = vec;
				m_RotateVec[ActionKey].push_back(Action);
			}
		}
	};

	//--------------------------------------------------------------------------------------
	//	class Action : public Component ;
	//	用途: 自動状態変更コンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Actions::Actions(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Actions::~Actions() {}

	//アクセサ
	bool Actions::IsLooped()const { return pImpl->m_Looped; }
	bool Actions::GetLooped()const { return pImpl->m_Looped; }
	void Actions::SetLooped(bool b) { pImpl->m_Looped = b; }

	bool Actions::IsArrived()const {
		bool retScale = true;
		if (pImpl->m_ScaleVec[pImpl->m_CurrntMapKey].size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_ScaleArrived) {
				retScale = false;
			}
		}
		bool retRot = true;
		if (pImpl->m_RotateVec[pImpl->m_CurrntMapKey].size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_RotateArrived) {
				retRot = false;
			}
		}
		bool retMove = true;
		if (pImpl->m_MoveVec[pImpl->m_CurrntMapKey].size() > 0) {
			//アクションがあるが終了してない
			if (!pImpl->m_MoveArrived) {
				retMove = false;
			}
		}
		return
			retScale &&
			retRot &&
			retMove;
	}
	bool Actions::GetArrived()const { return IsArrived(); }


	size_t Actions::GetScaleActiveIndex() const {
		if (pImpl->m_ScaleVec[pImpl->m_CurrntMapKey].size() <= 0) {
			throw BaseException(
				L"スケールアクションが登録されていません",
				L"if (pImpl->m_ScaleVec.size() <= 0)",
				L"Actions::GetScaleActiveIndex()"
			);
		}
		return pImpl->m_ScaleActiveIndex;
	}
	size_t Actions::GetRotateActiveIndex() const {
		if (pImpl->m_RotateVec[pImpl->m_CurrntMapKey].size() <= 0) {
			throw BaseException(
				L"回転アクションが登録されていません",
				L"if (pImpl->m_RotateVec.size() <= 0)",
				L"Actions::GetRotateActiveIndex()"
			);
		}
		return pImpl->m_RotateActiveIndex;
	}
	size_t Actions::GetMoveActiveIndex() const {
		if (pImpl->m_MoveVec[pImpl->m_CurrntMapKey].size() <= 0) {
			throw BaseException(
				L"移動アクションが登録されていません",
				L"if (pImpl->m_MoveVec.size() <= 0)",
				L"Actions::GetMoveActiveIndex()"
			);
		}
		return pImpl->m_MoveActiveIndex;
	}

	//現在の速度
	//Moveコンポーネントがない場合はbsm::Vec3(0,0,0)を返す

	bsm::Vec3 Actions::GetVelocity()const {
		if (pImpl->m_MoveVec.size() <= 0) {
			return bsm::Vec3(0, 0, 0);
		}
		else {
			auto Ptr = dynamic_pointer_cast<MoveComponent>(pImpl->m_MoveVec[pImpl->m_CurrntMapKey][pImpl->m_MoveActiveIndex]);
			if (Ptr) {
				return Ptr->GetVelocity();
			}
			else {
				return bsm::Vec3(0, 0, 0);
			}
		}
	}



	//操作
	shared_ptr<ScaleTo> Actions::AddScaleTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetScale, Rate);
		pImpl->AddScaleVec(ActionKey,Ptr);
		return Ptr;
	}
	shared_ptr<ScaleBy> Actions::AddScaleBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalScale, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<ScaleBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalScale, Rate);
		pImpl->AddScaleVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<ActionInterval> Actions::AddScaleInterval(wstring ActionKey,float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->AddScaleVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Actions::GetScaleComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_ScaleVec[pImpl->m_CurrntMapKey].size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_ScaleVec.size())",
				L"Actions::GetScaleComponent()"
			);
		}
		return pImpl->m_ScaleVec[pImpl->m_CurrntMapKey][TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Actions::GetScaleVec() const {
		return pImpl->m_ScaleVec[pImpl->m_CurrntMapKey];
	}


	shared_ptr<RotateTo> Actions::AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetRotate) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetRotate);
		pImpl->AddRotateVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<RotateTo> Actions::AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Quat& TargetQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetQuaternion);
		pImpl->AddRotateVec(ActionKey, Ptr);
		return Ptr;
	}


	shared_ptr<RotateBy> Actions::AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalRotate) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalRotate);
		pImpl->AddRotateVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<RotateBy> Actions::AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Quat& LocalQuaternion) {
		auto Ptr = ObjectFactory::Create<RotateBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalQuaternion);
		pImpl->AddRotateVec(ActionKey, Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Actions::AddRotateInterval(wstring ActionKey ,float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->AddRotateVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Actions::GetRotateComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_RotateVec[pImpl->m_CurrntMapKey].size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_RotateVec.size())",
				L"Actions::GetRotateComponent()"
			);
		}
		return pImpl->m_RotateVec[pImpl->m_CurrntMapKey][TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Actions::GetRotateVec() const {
		return pImpl->m_RotateVec[pImpl->m_CurrntMapKey];
	}

	shared_ptr<MoveTo> Actions::AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, Rate);
		pImpl->AddMoveVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<MoveTo> Actions::AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveTo>(GetGameObject());
		Ptr->SetParams(TotalTime, TargetPosition, RateX, RateY, RateZ);
		pImpl->AddMoveVec(ActionKey, Ptr);
		return Ptr;
	}


	shared_ptr<MoveBy> Actions::AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate Rate) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, Rate);
		pImpl->AddMoveVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<MoveBy> Actions::AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ) {
		auto Ptr = ObjectFactory::Create<MoveBy>(GetGameObject());
		Ptr->SetParams(TotalTime, LocalVector, RateX, RateY, RateZ);
		pImpl->AddMoveVec(ActionKey, Ptr);
		return Ptr;
	}


	shared_ptr<ActionInterval> Actions::AddMoveInterval(wstring ActionKey,float TotalTime) {
		auto Ptr = ObjectFactory::Create<ActionInterval>(GetGameObject());
		Ptr->SetTotalTime(TotalTime);
		pImpl->AddMoveVec(ActionKey, Ptr);
		return Ptr;
	}

	shared_ptr<ActionComponent> Actions::GetMoveComponent(size_t TargetIndex)const {
		if (TargetIndex >= pImpl->m_MoveVec[pImpl->m_CurrntMapKey].size()) {
			throw BaseException(
				L"インデックスが範囲外です",
				L"if (TargetIndex >= pImpl->m_MoveVec.size())",
				L"Actions::GetMoveComponent()"
			);
		}
		return pImpl->m_MoveVec[pImpl->m_CurrntMapKey][TargetIndex];
	}

	vector<shared_ptr<ActionComponent>>& Actions::GetMoveVec() const {
		return pImpl->m_MoveVec[pImpl->m_CurrntMapKey];
	}
	//すべてのアクションをクリアする
	void Actions::AllActionClear() {
		pImpl->m_ScaleVec[pImpl->m_CurrntMapKey].clear();
		pImpl->m_RotateVec[pImpl->m_CurrntMapKey].clear();
		pImpl->m_MoveVec[pImpl->m_CurrntMapKey].clear();

		pImpl->m_Looped = false;
		pImpl->m_ScaleArrived = false;
		pImpl->m_RotateArrived = false;
		pImpl->m_MoveArrived = false;
		pImpl->m_ScaleActiveIndex = 0;
		pImpl->m_RotateActiveIndex = 0;
		pImpl->m_MoveActiveIndex = 0;
	}



	void Actions::RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0) {
			TgtIndex = 0;
			TgtVector[TgtIndex]->Run();
		}
	}
	void Actions::Run(wstring ActionKey, const bool bLoopActive) {
		pImpl->SetCurrntActionKey(ActionKey);
		SetLooped(bLoopActive);
		RunSub(pImpl->m_ScaleVec[pImpl->m_CurrntMapKey], pImpl->m_ScaleActiveIndex);
		RunSub(pImpl->m_RotateVec[pImpl->m_CurrntMapKey], pImpl->m_RotateActiveIndex);
		RunSub(pImpl->m_MoveVec[pImpl->m_CurrntMapKey], pImpl->m_MoveActiveIndex);
	}

	void Actions::StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->Stop();
		}
	}
	void Actions::Stop() {
		StopSub(pImpl->m_ScaleVec[pImpl->m_CurrntMapKey], pImpl->m_ScaleActiveIndex);
		StopSub(pImpl->m_RotateVec[pImpl->m_CurrntMapKey], pImpl->m_RotateActiveIndex);
		StopSub(pImpl->m_MoveVec[pImpl->m_CurrntMapKey], pImpl->m_MoveActiveIndex);
	}

	void Actions::ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			TgtVector[TgtIndex]->ReStart();
		}
	}

	void Actions::ReStart() {
		ReStartSub(pImpl->m_ScaleVec[pImpl->m_CurrntMapKey], pImpl->m_ScaleActiveIndex);
		ReStartSub(pImpl->m_RotateVec[pImpl->m_CurrntMapKey], pImpl->m_RotateActiveIndex);
		ReStartSub(pImpl->m_MoveVec[pImpl->m_CurrntMapKey], pImpl->m_MoveActiveIndex);
	}

	bool Actions::UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex) {
		bool ret = false;
		if (TgtVector.size() > 0 && TgtIndex < TgtVector.size()) {
			if (TgtVector[TgtIndex]->IsUpdateActive()) {
				TgtVector[TgtIndex]->OnUpdate();
				if (TgtVector[TgtIndex]->IsArrived()) {
					//到着した
					//インデックスを加算
					TgtIndex++;
					if (TgtIndex >= TgtVector.size()) {
						//範囲外なら
						if (pImpl->m_Looped) {
							TgtIndex = 0;
							TgtVector[TgtIndex]->Run();
						}
						else {
							TgtIndex = TgtVector.size() - 1;
							//Runは行わない
							//到着した
							ret = true;
						}
					}
					else {
						TgtVector[TgtIndex]->Run();
					}
				}
			}
		}
		return ret;
	}
	//更新
	void Actions::OnUpdate() {
		pImpl->m_ScaleArrived = UpdateSub(pImpl->m_ScaleVec[pImpl->m_CurrntMapKey], pImpl->m_ScaleActiveIndex);
		pImpl->m_RotateArrived = UpdateSub(pImpl->m_RotateVec[pImpl->m_CurrntMapKey], pImpl->m_RotateActiveIndex);
		pImpl->m_MoveArrived = UpdateSub(pImpl->m_MoveVec[pImpl->m_CurrntMapKey], pImpl->m_MoveActiveIndex);
	}

}

//end basecross

