
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate(){
		try {
			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
			//PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStageHurukawa");
			//PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStageShogo");

		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();

		}
		if (event->m_MsgStr == L"ToGameStageShogo") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStageShogo>();

		}
		if (event->m_MsgStr == L"ToGameStageHurukawa") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStageHurukawa>();

		}
		if (event->m_MsgStr == L"ToStagechoice") {
			//最初のアクティブステージの設定
			ResetActiveStage<Stagechoice>();

		}
		if (event->m_MsgStr == L"ToSelectStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<SelectStage>();

		}
	}

}
//end basecross
