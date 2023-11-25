#pragma once
#include "SFML_Custom.h"
#include "GameData.h"

using namespace SFML_Custom;
using namespace Collision;
using namespace GameClass;

namespace Game
{
	void ProcessInitiate()
	{
		ResetAllManager();//시스템 매니저 초기화
		wm->StartTimer();
	}

	void ProcessFrame()		//Game Logic을 실행
	{
		em->DoAllProcess();
		wm->WorldProcess();
		em->CheckEnemySpace();
		em->CheckEnemyHit();
		lm->WeaponProcess();
		lm->LevelProcess();
		um->UiProcess();
	}
	void GraphicFrame()		//Resource를 Draw
	{
		sm->ClearSoundList();

		cm->RefreshCamera();
		em->DrawAllProcess();
	}
	void InterfaceFrame()	//User Interface를 Draw
	{
		lm->DrawProcess();
		um->UiDraw();
	}

	void TotalSystem()
	{
		FrameTimeRefresh();

		if (IsAbleToDraw()) {
			nowWindow->clear(wm->worldColor);
			GraphicFrame();
			InterfaceFrame();
			timeFromDraw = 0;
			nowWindow->display();
		}
		if (IsAbleToProcess()) {
			ProcessFrame();
			timeToProcess -= 1000 / frameRatio;	//오차가 발생한다. 괜찮을까?
		}
	}
}
