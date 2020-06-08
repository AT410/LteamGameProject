/*!
@file ProjectBehavior.h
@brief プロジェク定義のComponentクラス
*/

#pragma once
#include "stdafx.h"

namespace basecross 
{
	//----------------------------------------------------------------------------
	//アクションクラスの拡張
	//----------------------------------------------------------------------------
	class Actions :public Component
	{
		void RunSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void StopSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		void ReStartSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
		bool UpdateSub(vector<shared_ptr<ActionComponent> >& TgtVector, size_t& TgtIndex);
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Actions(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Actions();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ループするかどうかを得る
		@return	ループするならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsLooped()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ループするかどうかを得る
		@return	ループするならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetLooped()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ループするかどうかを設定する
		@param[in]	b	ループするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLooped(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	到着したかどうかを得る<br />
		ループなしで到着したら真になる
		@return	到着したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsArrived()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	到着したかどうかを得る<br />
		ループなしで到着したら真になる
		@return	到着したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetArrived()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の速度を得る<br />
		Moveコンポーネントがない場合はbsm::Vec3(0,0,0)を返す
		@return	現在の速度
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetVelocity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリングアクションの現在のインデックスを得る<br />
		スケーリングアクションが存在しなければ例外
		@return	現在のインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t GetScaleActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転アクションの現在のインデックスを得る<br />
		回転アクションが存在しなければ例外
		@return	現在のインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRotateActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	移動アクションの現在のインデックスを得る<br />
		移動アクションが存在しなければ例外
		@return	現在のインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMoveActiveIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ScaleToアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	TargetScale	目的のスケール
		@param[in]	Rate = Lerp::Linear	補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ScaleTo> AddScaleTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetScale, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ScaleByアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	LocalScale	目的の相対スケール
		@param[in]	Rate = Lerp::Linear	補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ScaleBy> AddScaleBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalScale, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ScaleIntervalアクションを追加する
		@param[in]	TotalTime	かける時間
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddScaleInterval(wstring ActionKey, float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定したインデックスのスケールコンポーネントを得る
		@param[in]	TargetIndex	取得するインデックス
		@return　指定したインデックスのスケールコンポーネント（アクションのポインタ）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetScaleComponent(size_t TargetIndex) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型にキャストした指定したインデックスのスケールコンポーネントを得る
		@tparam	T	スケール型
		@param[in]	TargetIndex	取得するインデックス
		@return	指定したインデックスのT型のコンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetScale(size_t TargetIndex) const {
			auto Ptr = GetScaleComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"Action::GetScale()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケールコンポーネントの配列を得る<br/>
		実際の配列が返るので慎重に操作すること
		@return	スケールコンポーネントの配列の参照
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetScaleVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateToアクションを追加する<br/>
		内部で持っているのはクオータニオンなので、変換してセットする
		@param[in]	TotalTime	かける時間
		@param[in]	TargetRotate	目的の回転
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateTo> AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetRotate);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateToアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	Targetbsm::Quat	目的の回転クオータニオン
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateTo> AddRotateTo(wstring ActionKey,float TotalTime, const bsm::Quat& TargetQuaternion);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RotateByアクションを追加する<br/>
		内部で持っているのはクオータニオンなので、変換してセットする
		@param[in]	TotalTime	かける時間
		@param[in]	LocalRotate	目的の相対回転
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateBy> AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalRotate);
		//--------------------------------------------------------------------------------------
		/*!
		@brief　RotateByアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	Localbsm::Quat	目的の相対回転クオータニオン
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RotateBy> AddRotateBy(wstring ActionKey,float TotalTime, const bsm::Quat& LocalQuaternion);
		//--------------------------------------------------------------------------------------
		/*!
		@brief RotateIntervalアクションを追加する
		@param[in]	TotalTime	かける時間
		@return　追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddRotateInterval(wstring ActionKey, float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定したインデックスの回転コンポーネントを得る
		@param[in]	TargetIndex	取得するインデックス
		@return	指定したインデックスの回転コンポーネント（アクションのポインタ）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetRotateComponent(size_t TargetIndex)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型にキャストした指定したインデックスの回転コンポーネントを得る
		@tparam	T	回転型
		@param[in]	TargetIndex	取得するインデックス
		@return	指定したインデックスのT型のコンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetRotate(size_t TargetIndex)const {
			auto Ptr = GetRotateComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"Action::GetRotate()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転コンポーネントの配列を得る<br/>
		実際の配列が返るので慎重に操作すること
		@return	回転コンポーネントの配列の参照
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetRotateVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveToアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	TargetPosition	目的の位置
		@param[in]	Rate = Lerp::Linear	補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveTo> AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveToアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	TargetPosition	目的の位置
		@param[in]	RateX	X補間方法
		@param[in]	RateY	Y補間方法
		@param[in]	RateZ	Z補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveTo> AddMoveTo(wstring ActionKey,float TotalTime, const bsm::Vec3& TargetPosition, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveByアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	LocalVector	目的の相対位置
		@param[in]	Rate = Lerp::Linear	補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveBy> AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate Rate = Lerp::Linear);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveByアクションを追加する
		@param[in]	TotalTime	かける時間
		@param[in]	LocalVector	目的の相対位置
		@param[in]	RateX	X補間方法
		@param[in]	RateY	Y補間方法
		@param[in]	RateZ	Z補間方法
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MoveBy> AddMoveBy(wstring ActionKey,float TotalTime, const bsm::Vec3& LocalVector, Lerp::rate RateX, Lerp::rate RateY, Lerp::rate RateZ);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	MoveIntervalアクションを追加する
		@param[in]	TotalTime	かける時間
		@return	追加したアクションのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionInterval> AddMoveInterval(wstring ActionKey,float TotalTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定したインデックスの移動コンポーネントを得る
		@param[in]	TargetIndex	取得するインデックス
		@return	指定したインデックスの移動コンポーネント（アクションのポインタ）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ActionComponent> GetMoveComponent(size_t TargetIndex)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief T型にキャストした指定したインデックスの移動コンポーネントを得る
		@tparam	T	移動型
		@param[in]	TargetIndex	取得するインデックス
		@return	指定したインデックスのT型のコンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetMove(size_t TargetIndex)const {
			auto Ptr = GetMoveComponent(TargetIndex);
			auto Ptr2 = dynamic_pointer_cast<T>(Ptr);
			if (!Ptr2) {
				throw BaseException(
					L"そのコンポーネントはT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"Action::GetMove()"
				);
			}
			return Ptr2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	移動コンポーネントの配列を得る<br/>
		実際の配列が返るので慎重に操作すること
		@return	移動コンポーネントの配列の参照
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<ActionComponent>>& GetMoveVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべてのアクションをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AllActionClear();
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクションを実行する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Run(wstring ActionKey, const bool bLoopActive = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief アクションを止める
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Stop();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクションを再スタートする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ReStart();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。空関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;

	};
}

//end basecross

