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
		ResetAllManager();//�ý��� �Ŵ��� �ʱ�ȭ
		wm->StartTimer();
	}

	void ProcessFrame()		//Game Logic�� ����
	{
		em->DoAllProcess();
		wm->WorldProcess();
		em->CheckEnemySpace();
		em->CheckEnemyHit();
		lm->WeaponProcess();
		lm->LevelProcess();
		um->UiProcess();
	}
	void GraphicFrame()		//Resource�� Draw
	{
		sm->ClearSoundList();

		cm->RefreshCamera();
		em->DrawAllProcess();
	}
	void InterfaceFrame()	//User Interface�� Draw
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
			timeToProcess -= 1000 / frameRatio;	//������ �߻��Ѵ�. ��������?
		}
	}
}
