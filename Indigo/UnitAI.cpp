#include"Unit.h"
#include"Header.h"

void	Unit::unitAI()
{
	//Timerの初期化
	Timer2 = 0;
	Timer1 = 0;
	switch (Status)
	{
	case DoNothing: //食後、起床後に個別処理に復帰するための場所
		//お腹空いているのかという処理
		if (NumEat == 0 || (NumEat == 1 && sec > 43200) || (NumEat == 2 && isNight())) setStatus(TakeMeal);
		else if (isNight()) setStatus(GoToSleep);
		else
		{

			switch (getJob())
			{
			case Labor:		setPenalty(3600); break;
			case Farmer:	setStatus(WorkAsFarmer); break;
			case Woodcutter:setStatus(WorkAsWoodcutter); break;
			case Builder:	setStatus(WorkAsBuilder); break;
			case Brewer:	setStatus(WorkAsBrewer); break;
			default:		setPenalty(600); break;
			}
		}
		break;

	case IsSleeping: //寝ている→起きた場合
		setStatus(DoNothing);
		break;



		//持っている物を納品する
	case Deliver:
		switch (aiProgress)
		{
		case 0:
			switch (getJob())
			{
			case Farmer:	//市場に預けるタイプ
				if (goNearMarket()) aiProgress = 1;
				else setPenalty(3600);
				break;
			default:		//自分の施設に預けるタイプ
				if (goTo(getJobFacility())) aiProgress = 1;
				else setPenalty(3600);
				break;
			}
			break;

		case 1:
			sendItemToFacility();
			setStatus(DoNothing);
			break;

		default: Println(L"D Progress Error"); setPenalty(3600); break;
		}
		break;



		/*
		食事を取る
		家にRiceが無ければ取りに行く
		家にWoodが無ければ取りに行く
		*/
	case TakeMeal:
	{
		Facility* f;
		switch (aiProgress)
		{
		case 0:	//アイテムを預ける
			if (!hasHomeFacility())
			{
				f = searchItem(Rice);
				if (f == NULL) setPenalty(3600);
				else
				{
					if (getStandingChip().getOwnFacility() == f)
					{
						receiveFromFacility(Rice, 1);
						aiProgress = 8;
						goRoomRandom();
					}
					else goTo(f);
				}
			}
			else if (NumItems > 0) setStatus(Deliver);	//預けてからの処理
			else if (getHomeFacility()->getNumItem(Rice) == 0) aiProgress = 1;
			else if (getHomeFacility()->getNumItem(Wood) == 0) aiProgress = 2;
			else aiProgress = 7;
			break;

		case 1:	//Riceを探し、取りに行く
			f = searchItem(Rice);
			if (f == NULL) setPenalty(3600);
			else { goTo(f); aiProgress = 3; }
			break;
		case 2:	//Woodを探し、取りに行く
			f = searchItem(Wood);
			if (f == NULL) setPenalty(3600);
			else { goTo(f); aiProgress = 4; }
			break;
		case 3:	//Riceを取得、そして家に戻る
			if (receiveFromFacility(Rice, 3) == 0) aiProgress = 1;	//もし、無ければ再探索
			else if (goTo(getHomeFacility())) aiProgress = 6;
			else setPenalty(3600);
			break;
		case 4:	//Woodを取得、そして家に戻る
			if (receiveFromFacility(Wood, 3) == 0) aiProgress = 2;	//もし、無ければ再探索
			else if (goTo(getHomeFacility())) aiProgress = 6;
			else setPenalty(3600);
			break;
		case 6:	//家に帰ってきて、荷物を落とす
			if (getNumItem(Rice) > 0) sendItemToFacility(Rice);
			if (getNumItem(Wood) > 0) sendItemToFacility(Wood);
			aiProgress = 0;
			break;
		case 7:	//Riceの消費タイマーをセット
			if (getStandingChip().getOwnFacility() == getHomeFacility())
			{
				receiveFromFacility(Rice, 1);
				receiveFromFacility(Wood, 1);
				aiProgress = 8;
				goRoomRandom();
			}
			else if (!goTo(getHomeFacility())) setPenalty(3600);
			break;
		case 8:
			Timer1 = 60;
			aiProgress = 9;
			break;
		case 9:	//Riceを消費
			addItem(Rice, -1);
			addItem(Wood, -1);
			NumEat++;
			setStatus(DoNothing);
			break;

		default: Println(L"TM Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	//寝る
	case GoToSleep:
	{
		switch (aiProgress)
		{
		case 0:	//家に戻る
			if (NumItems > 0) setStatus(Deliver);
			else
			{
				goTo(getHomeFacility());	//ない場合はその場で続行
				aiProgress = 1;
			}
			break;
		case 1:
			goRoomRandom();
			aiProgress = 2;
			break;
		case 2:
			IsSleep = true;
			setStatus(IsSleeping);
			break;
		default: Println(L"GTS Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	//小麦農家
	case WorkAsFarmer:
	{
		switch (aiProgress)
		{
		case 0:
			if (goTo(getJobFacility())) aiProgress = 1;
			else setPenalty(3600);
			break;
		case 1:
			goRoomRandom();
			aiProgress = 2;
			break;
		case 2:
			Timer1 = 3600;
			aiProgress = 3;
			break;
		case 3:
			produceItem(Rice, 1);
			setStatus(DoNothing);
			break;
		default: Println(L"WAF Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	//木こり
	case WorkAsWoodcutter:
	{
		switch (aiProgress)
		{
		case 0:
			if (getJobFacility()->getNumItem(Wood) > 1000) setPenalty(3600);
			if (goToNearForest()) aiProgress = 2;
			else setPenalty(3600);
			break;
		case 2:
			Timer1 = 300;
			aiProgress = 3;
			break;
		case 3:
			produceItem(Wood, 1);
			setStatus(DoNothing);
			break;
		default: Println(L"WAW Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	//

	//大工
	case WorkAsBuilder:
	{
		Facility* f;
		if (!hasTarFacility() || getTarFacility()->BuildProgress == 0 || !isConeected(getTarFacility()->getEntranceChip())) setTarFacility(searchPlannedSite());
		if (getTarFacility() == NULL) aiProgress = 0;
		switch (aiProgress)
		{
		case 0:	//仕事場の確認
			if (!hasTarFacility()) setPenalty(3600);
			else
			{
				if (getNumItem(Wood) == 0) aiProgress = 1;
				else aiProgress = 3;
			}
			break;

		case 1:	//木材を持ってくる
			f = searchItem(Wood);
			if (f == NULL) setPenalty(3600);
			else
			{
				goTo(f);
				aiProgress = 2;
			}
			break;

		case 2:
			if (receiveFromFacility(Wood, 10) == 0) aiProgress = 1;
			else aiProgress = 3;
			break;

		case 3:
			if (goTo(getTarFacility())) aiProgress = 4;
			else aiProgress = 0;
			break;

		case 4:
			goRoomRandom();
			aiProgress = 5;
			break;
		case 5:
			Timer1 = 60;
			aiProgress = 6;
			break;
		case 6:
			addItem(Wood, -1);
			getTarFacility()->BuildProgress--;
			setStatus(DoNothing);
			break;
		default: Println(L"WAB1 Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	//醸造家
	case WorkAsBrewer:
	{
		Facility* f;
		switch (aiProgress)
		{
		case 0:	//仕事場の確認
			if (getNumItem(Wood) == 0 && receiveFromFacility(Wood, 10) == 0) aiProgress = 1;
			else if (getNumItem(Rice) == 0 && receiveFromFacility(Rice, 10) == 0) aiProgress = 2;
			else
			{
				if (goTo(getJobFacility())) aiProgress = 3;
				else setPenalty(3600);
			}
			break;

		case 1:	//木材を持ってくる
			f = searchItem(Wood);
			if (f == NULL) setPenalty(3600);
			else
			{
				goTo(f);
				aiProgress = 0;
			}
			break;

		case 2:
			f = searchItem(Rice);
			if (f == NULL) setPenalty(3600);
			else
			{
				goTo(f);
				aiProgress = 0;
			}
			break;

		case 3:
			goRoomRandom();
			aiProgress = 5;
			break;
		case 5:
			Timer1 = 60;
			aiProgress = 4;
			break;
		case 4:
			produceItem(Sake, 1);
			addItem(Wood, -1);
			addItem(Rice, -1);
			setStatus(DoNothing);
			break;
		default: Println(L"WAB2 Progress Error"); setPenalty(3600); break;
		}
		break;
	}

	default:
		Println(L"UnitAI Not Found Error"); setPenalty(3600);
		break;
	}
}