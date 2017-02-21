#include"Unit.h"
#include"Header.h"

String Unit::getMessage()
{
	switch (Status)
	{
	case DoNothing:		return L"何もしていない";
	case IsSleeping:	return L"寝てる";
	case TakeMeal:		return L"食事中...";
	case GoToSleep:		return L"寝に行く";
	case WorkAsBuilder:
		break;
	case WorkAsFarmer:
		break;
	case WorkAsWoodcutter:
		break;
	case WorkAsBrewer:
		break;
	default:
		break;
	}
	/*
	switch (Status)
	{
	case DoNothing:		return L"何もしてない";
	case RestInHouse:	return L"家で休んでる";
	case EatBread:		return L"パン食べてる";
	case BakeBread:		return L"パンを焼いてる";
	case TakeWheat:		return L"小麦を取りに行く";
	case TakeWood:		return L"木材を取りに行く";
	case TakeBread:		return L"パンを取りに行く";
	case TakeVenion:	return L"鹿肉を取りに行く";
	case TakeLeather:	return L"毛皮を取りに行く";
	case WaitForWheat:	return L"小麦の入荷待ち";
	case WaitForWood:	return L"木材の入荷待ち";
	case WaitForBread:	return L"パンの入荷待ち";
	case WaitForVenion:	return L"鹿肉の入荷待ち";
	case WaitForLeather:return L"毛皮の入荷待ち";
	case CantGoToJobHouse:return L"仕事場に行けない";
	case CantGoToHome:	return L"家に行けない";
	case CantGoToMarket:return L"行ける市場がない";
	case CantGoToForest:return L"森への道がない";
	case NoRoomForBread:return L"パンの置き場がない";
	case SendItemToMarket:return L"市場に預けに行く";
	case IsSleeping:	return L"ぐー、ぐー";
	case GoToHome:		return L"家に行く";
	case GoToJobHouse:	return L"仕事場に行く";
	case GoToMarket:	return L"市場に行く";
	case GoToForest:	return L"森林に行く";
	case GoToPlannedSite:return L"建設予定地に行く";
	case TakeRest:		return L"休憩中...";
	case NoHome:		return L"家がない！";
	case NoJobHouse:	return L"仕事場がない！";
	case NoForest:		return L"森林がない！";
	case NoPlannedSite: return L"建設予定地がない！";
	case ReapWheat:		return L"小麦を収穫中...";
	case PileUp:		return L"建設中...";
	case CutTree:		return L"伐採中...";
	case Hunting:		return L"狩猟中...";
	case Brewing:		return L"醸造中...";
	case NoRoomForSake: return L"酒の置き場がない！";
	default:			return L"メッセージなし";
	}*/
	return L"Nothing";
}