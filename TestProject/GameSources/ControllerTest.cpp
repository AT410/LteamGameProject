#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	void DebugTest::OnCreate()
	{
		wstring mediaPath;
		App::GetApp()->GetDataDirectory(mediaPath);

		vector<wstring> FontPaths;
		FontPaths.clear();
		FontPaths.push_back(mediaPath + L"001Shirokuma-Regular.otf");
		FontPaths.push_back(mediaPath + L"memoir.otf");
		FontPaths.push_back(mediaPath + L"Zomzi.TTF");

		auto ptrString = AddComponent<OriginalStringSprite>(FontPaths,L"暗黒ゾン字");
		ptrString->SetText(L"テストですよ。");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 50.0f, 640.0f, 480.0f));

		//各パフォーマンスを得る
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);
	}

	void DebugTest::OnUpdate()
	{
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring fpsStr(L"テストですよ。\nFPS: ");
		fpsStr += Util::UintToWStr(fps);
		fpsStr += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		fpsStr += Util::FloatToWStr(ElapsedTime);
		fpsStr += L"\n";

		wstring updatePerStr(L"UpdatePerformance:\t");
		updatePerStr += Util::FloatToWStr(GetStage()->GetUpdatePerformanceTime());
		updatePerStr += L"\tmillisecond\n";

		wstring drawPerStr(L"DrawPerformance:\t");
		drawPerStr += Util::FloatToWStr(GetStage()->GetDrawPerformanceTime());
		drawPerStr += L"\tmillisecond\n";

		wstring collPerStr(L"CollisionPerform:\t");
		collPerStr += Util::FloatToWStr(GetStage()->GetCollisionPerformanceTime(), 5);
		collPerStr += L"\tmillisecond\n";

		wstring collMiscStr(L"ColMiscPerform:\t");
		collMiscStr += Util::FloatToWStr(GetStage()->GetCollisionManager()->GetMiscPerformanceTime(), 5);
		collMiscStr += L"\tmillisecond\n";

		wstring collTernCountStr(L"CollisionCountOfTern:\t");
		collTernCountStr += Util::UintToWStr(GetStage()->GetCollisionManager()->GetCollisionCountOfTern());
		collTernCountStr += L"\n";

		wstring ObjCount(L"ObjectCount:\t");
		ObjCount += Util::UintToWStr((UINT)GetStage()->GetGameObjectVec().size());
		ObjCount += L"\n";

		wstring str = fpsStr + updatePerStr + drawPerStr + collPerStr + collMiscStr
			+ collTernCountStr + ObjCount;

		//文字列コンポーネントの取得
		auto ptrString = GetComponent<OriginalStringSprite>();
		ptrString->SetText(str);

	}
}