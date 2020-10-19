/*!
@file StringComponents.h
@brief 文字列描画コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	StringSpriteコンポーネント(文字列表示)
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StringSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@enum TextAlignment
		*/
		//--------------------------------------------------------------------------------------
		enum TextAlignment{
			//左合わせ
			m_Left,
			//中央合わせ
			m_Center,
			//右合わせ
			m_Right
		};
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントを設定する
		@param[in]	FontName フォント名
		@param[in]	FontSize フォントサイズ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを得る
		@return	テキストのアラインメント
		*/
		//--------------------------------------------------------------------------------------
		StringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを設定する
		@param[in]	Alignment アラインメント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを得る
		@return	テキスト
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを設定する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを追加する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を得る
		@return	フォント色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を設定する
		@param[in]	Col フォント色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント名を得る
		@return	フォント名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントサイズを得る
		@return	フォントサイズ
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を得る
		@return	背景色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を設定する
		@param[in]	Col 背景色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを得る
		@return	背景のテキストマージン
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを設定する
		@param[in]	p 背景のテキストマージン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの幅を取得する
		@return	テキストブロックの幅
		*/
		//--------------------------------------------------------------------------------------
		float GetTextBlockWidth() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの幅を設定する
		@param[in]	f テキストブロックの幅
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextBlockWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの高さを取得する
		@return	テキストブロックの高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetTextBlockHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの高さを設定する
		@param[in]	f テキストブロックの高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextBlockHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストレイアウトを取得する
		@return	テキストレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストフォーマットを取得する
		@return	テキストフォーマット
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストメトリクスを取得する
		@return	テキストメトリクス
		*/
		//--------------------------------------------------------------------------------------
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を取得する
		@return	スタート位置
		*/
		//--------------------------------------------------------------------------------------
		const Point2D<float>& GetStartPosition() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を設定する
		@param[in]	pos スタート位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetStartPosition(const Point2D<float>& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストの矩形を取得する
		@return	テキストの矩形
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<float> GetTextRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストの矩形を設定する
		@param[in]	rect テキストの矩形
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextRect(const Rect2D<float>& rect);
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	複数ブロックの文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class MultiStringSprite : public Component {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiStringSprite();
		//アクセサ
		//ブロック共通
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントを設定する
		@param[in]	FontName フォント名
		@param[in]	FontSize フォントの大きさ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントの色を取得する
		@return	フォントの色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を設定する
		@param[in]	Col フォント色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント名を取得する
		@return	フォント名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントサイズを取得する
		@return	フォントサイズ
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を取得する
		@return	背景色		
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を設定する
		@param[in]	Col 背景色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストマージンを取得する
		@return	テキストマージン
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストマージンを設定する
		@param[in]	p テキストマージン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストフォーマットを取得する
		@return	テキストフォーマット
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストアラインメントを取得する
		@return	テキストアラインメント
		*/
		//--------------------------------------------------------------------------------------
		StringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストアラインメントを設定する
		@param[in]	Alignment テキストアラインメント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		//ブロック単位
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキスト
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText(size_t Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを設定する
		@param[in]	Index ブロックのインデックス
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを追加する
		@param[in]	Index ブロックのインデックス
		@param[in]	str 追加するテキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddText(size_t Index, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックを追加する
		@param[in]	Block ブロック
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックを更新する
		@param[in]	Index ブロックのインデックス
		@param[in]	Block ブロック
		@param[in]	str テキスト
		@param[in]	Clip クリッピングするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearTextBlock();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストレイアウトを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキストレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextLayout>& GetTextLayout(size_t Index)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストメトリクスを取得する
		@param[in]	Index ブロックのインデックス
		@return	テキストメトリクス
		*/
		//--------------------------------------------------------------------------------------
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics(size_t Index) const;
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

#ifndef NDEBUG
	//--------------------------------------------------------------------------------------
	///	自作フォント読込文字列表示
	//--------------------------------------------------------------------------------------
	class OriginalStringSprite :public Component
	{
	public:
		explicit OriginalStringSprite(const shared_ptr<GameObject>&GameObjectPtr,const vector<wstring> FontFileName,const wstring DefaultFonts);

		virtual ~OriginalStringSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@enum TextAlignment
		*/
		//--------------------------------------------------------------------------------------
		enum TextAlignment {
			//左合わせ
			m_Left,
			//中央合わせ
			m_Center,
			//右合わせ
			m_Right
		};
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントを設定する
		@param[in]	FontName フォント名
		@param[in]	FontSize フォントサイズ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを得る
		@return	テキストのアラインメント
		*/
		//--------------------------------------------------------------------------------------
		OriginalStringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを設定する
		@param[in]	Alignment アラインメント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(OriginalStringSprite::TextAlignment Alignment);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを得る
		@return	テキスト
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを設定する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを追加する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を得る
		@return	フォント色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を設定する
		@param[in]	Col フォント色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント名を得る
		@return	フォント名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントサイズを得る
		@return	フォントサイズ
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を得る
		@return	背景色
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Col4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を設定する
		@param[in]	Col 背景色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const bsm::Col4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを得る
		@return	背景のテキストマージン
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを設定する
		@param[in]	p 背景のテキストマージン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの幅を取得する
		@return	テキストブロックの幅
		*/
		//--------------------------------------------------------------------------------------
		float GetTextBlockWidth() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの幅を設定する
		@param[in]	f テキストブロックの幅
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextBlockWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの高さを取得する
		@return	テキストブロックの高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetTextBlockHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストブロックの高さを設定する
		@param[in]	f テキストブロックの高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextBlockHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストレイアウトを取得する
		@return	テキストレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストフォーマットを取得する
		@return	テキストフォーマット
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストメトリクスを取得する
		@return	テキストメトリクス
		*/
		//--------------------------------------------------------------------------------------
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を取得する
		@return	スタート位置
		*/
		//--------------------------------------------------------------------------------------
		const Point2D<float>& GetStartPosition() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スタート位置を設定する
		@param[in]	pos スタート位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetStartPosition(const Point2D<float>& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストの矩形を取得する
		@return	テキストの矩形
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<float> GetTextRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストの矩形を設定する
		@param[in]	rect テキストの矩形
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextRect(const Rect2D<float>& rect);
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
#endif // NDEBUG


}

//end basecross
