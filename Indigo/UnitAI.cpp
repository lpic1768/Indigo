#include"Basic.h"
void	Unit::unitAI()
{
	if (needFood && progress == 0)	//H—¿‚ð—~‚·‚é‚Æ‚«‚Ìˆ—
	{
		if (getPlace() != NULL && getPlace()->items.getNumItem(Wheat) != 0) { getPlace()->items.pullItem(Wheat, 1); needFood = false; if (culturePoint > 0) { culturePoint -= 1; } }
		else if (!goToTakeItem(Wheat)) timerMax = 3600;
	}
	else if (needSake && progress == 0)	//Žð‚ð—~‚·‚é‚Æ‚«‚Ìˆ—
	{
		if (getPlace() != NULL && getPlace()->items.getNumItem(Sake) != 0) { getPlace()->items.pullItem(Sake, 1); needSake = false; culturePoint += 4; }
		else if (!goToTakeItem(Sake)) needSake = false;	//’ú‚ß‚é
	}
	else if ((workStopPenalty || isNight()) && progress == 0)	//Q‚é
	{
		if (getChip(pos).getPlace() == home.getPlace() && home.getPlace() != NULL) timerMax = 1800;
		else { if (!goTo(GoToHome)) timerMax = 3600; }
	}
	else
	{
		switch (jobType)
		{
		case Laborer: workStopPenalty = true; break;
		case Farmer:
			switch (progress)
			{
			case 0:	//‰Šú•ªŠò
				progress = 1;
				break;
			case 1:
				if (getPlace() != home.getPlace() && !goTo(GoToHome)) { timerMax = 3600; }
				else progress = 2;
				break;
			case 2:
				if (items.getNumItem(Wheat) > 0) { home.getPlace()->items.addItem(Wheat, items.getNumItem(Wheat)); items.reset(10); }
				if (isNight()) progress = 0;
				else progress = 3;
				break;
			case 3:
				if (!goTo(GoToFarm)) { workStopPenalty = true; progress = 0; }
				else progress = 4;
				break;
			case 4:
				timerMax = 1800;
				progress = 5;
				break;
			case 5:
				if (getChip(pos).growth == 12)
				{
					getChip(pos).growth = 1;
					items.addItem(Wheat, 5);
				}
				else getChip(pos).growth++;
				getChip(pos).isUsedByUnit = false;
				if (items.getNumItem(Wheat) >= 10 || isNight()) progress = 1;
				else progress = 3;
				break;
			}
			break;
		case Lumberjack:
			switch (progress)
			{
			case 0:	//‰Šú•ªŠò
				progress = 1;
				break;
			case 1:
				if (getPlace() != home.getPlace() && !goTo(GoToHome)) { timerMax = 3600; }
				else progress = 2;
				break;
			case 2:
				if (items.getNumItem(Wood) > 0) { home.getPlace()->items.addItem(Wood, items.getNumItem(Wood)); items.reset(10); }
				if (isNight()) progress = 0;
				else progress = 3;
				break;
			case 3:
				if (!goTo(GoToForest)) { workStopPenalty = true; progress = 0; }
				else progress = 4;
				break;
			case 4:
				timerMax = 1800;
				progress = 5;
				break;
			case 5:
				if (getChip(pos).growth == 3) { getChip(pos).growth = 4; getChip(pos).numItem = 100; }
				items.addItem(Wood, 10);
				getChip(pos).numItem -= 10;
				if (getChip(pos).numItem == 0) getChip(pos).growth = 0;
				getChip(pos).isUsedByUnit = false;
				progress = 1;
				break;
			}
			break;
		case Merchant:
			timerMax = 3600;
			break;
		case Brewer:
			switch (progress)
			{
			case 0:	//‰Šú•ªŠò
				progress = 1;
				break;
			case 1:
				if (getPlace() != home.getPlace() && !goTo(GoToHome)) { timerMax = 3600; }
				else progress = 2;
				break;
			case 2:
				if (items.getNumItem(Wheat) > 0) { getPlace()->items.addItem(Wheat, items.getNumItem(Wheat)); items.pullItem(Wheat, items.getNumItem(Wheat)); }
				if (items.getNumItem(Wood) > 0) { getPlace()->items.addItem(Wood, items.getNumItem(Wood)); items.pullItem(Wood, items.getNumItem(Wood)); }
				if (getPlace()->items.getNumItem(Wood) < 4) { if (goToTakeItem(Wood)) { progress = 4; } else { workStopPenalty = true; progress = 0; } }
				else if (getPlace()->items.getNumItem(Wheat) < 4) { if (goToTakeItem(Wheat)) { progress = 3; } else { workStopPenalty = true; progress = 0; } }
				else
				{
					getPlace()->items.pullItem(Wheat, 4);
					getPlace()->items.pullItem(Wood, 4);
					timerMax = 1800;
					progress = 5;
				}
				break;
			case 3:	//Wheat‚ðŽæ‚è‚És‚­
				if (getPlace()->items.getNumItem(Wheat) < 4)
				{
					items.addItem(Wheat, getPlace()->items.getNumItem(Wheat));
					getPlace()->items.pullItem(Wheat, getPlace()->items.getNumItem(Wheat));
				}
				else
				{
					items.addItem(Wheat, 4);
					getPlace()->items.pullItem(Wheat, 4);
				}
				progress = 1;
				break;
			case 4:	//Wood‚ðŽæ‚è‚És‚­
				if (getPlace()->items.getNumItem(Wood) < 4)
				{
					items.addItem(Wood, getPlace()->items.getNumItem(Wood));
					getPlace()->items.pullItem(Wood, getPlace()->items.getNumItem(Wood));
				}
				else
				{
					items.addItem(Wood, 4);
					getPlace()->items.pullItem(Wood, 4);
				}
				progress = 1;
				break;
			case 5:
				getPlace()->items.addItem(Sake, 1);
				progress = 0;
				break;
			}
			break;
		default: workStopPenalty = true; break;
		}
	}
}