/*!
@file EfkInterface.h
@brief Effekseerエフェクトのインターフェイス
*/

#pragma once

#include "stdafx.h"

namespace basecross
{
	class EfkInterface;

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのエフェクト
	//--------------------------------------------------------------------------------------
	class EffectResource :public BaseResource
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		@param[in]	iface	インターフェイス
		@param[in]	filename	エフェクトファイル名
		*/
		//--------------------------------------------------------------------------------------
		EffectResource(const shared_ptr<EfkInterface>& iface, const wstring& filename,float magnification = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EffectResource();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
	private:
		wstring m_FileName;
		Effekseer::Effect* m_Effect;
		weak_ptr<EfkInterface> m_EfkInterface;
		friend class EfkPlay;
	};

	//--------------------------------------------------------------------------------------
	///	EffekseerエフェクトのPlayオブジェクト
	//--------------------------------------------------------------------------------------
	class EfkPlay : public ObjectInterface
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		@param[in]	effect	エフェクト
		@param[in]	Emitter	エミッター
		*/
		//--------------------------------------------------------------------------------------
		//EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter);
		EfkPlay(const wstring&key, const bsm::Vec3& Emitter);
		//追加
		EfkPlay(const shared_ptr<EffectResource>& effect, const bsm::Vec3& Emitter);

		virtual ~EfkPlay();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エフェクトをストップさせる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StopEffect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エフェクトを移動する
		@param[in]	Location	移動ベクトル（相対方向）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddLocation(const bsm::Vec3& Location);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エフェクトを移動する
		@param[in]	Location	移動ベクトル（絶対方向）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLocation(const bsm::Vec3& Location);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	変換行列を設定する
		@param[in]	Mat4x4	親要素の行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTransMat4x4(const bsm::Mat4x4& TransMat);
	private:
		::Effekseer::Handle m_Handle;
		weak_ptr<EfkInterface> m_EfkInterface;
	};

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのインターフェイス
	//--------------------------------------------------------------------------------------
	class EfkInterface :public ObjectInterface, public ShapeInterface 
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスラトクタ
		*/
		//--------------------------------------------------------------------------------------
		EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~EfkInterface();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューと射影行列を設定する
		@param[in]	view	ビュー行列
		@param[in]	proj	射影行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		::Effekseer::Manager* m_Manager;
		::EffekseerRenderer::Renderer*	m_Renderer;
		friend class EfkPlay;
		friend class EffectResource;
	};
}