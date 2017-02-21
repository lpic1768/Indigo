#include"Unit.h"
#include"Header.h"

void Unit::update()
{
	AP = actionPoint;	//APの付与

	for (;;)
	{
		if (getStandingChip().RoadLevel != 1) { erase(); return; }
		if (!isNight()) {
			if (IsSleep)
			{
				IsSleep = false;
				if (getJobFacility() == NULL)
				{
					Facility* f1 = NULL;
					int b = 0;
					for (auto& f2 : facilities)
					{
						if (f2.Enabled &&
							f2.getJoinedRegion() == getJoinedRegion() &&
							f2.NumUnitsMax - f2.NumUnits > 0 &&
							f2.IsJobFacility() &&
							(f2.getEntranceChip()->getCenterPos().distanceFrom(Pos) < b || f1 == NULL) &&
							isConeected(f2.getEntranceChip()))
						{
							f1 = &f2;
							b = f2.getEntranceChip()->getCenterPos().distanceFrom(Pos);
						}
					}
					if (f1 != NULL)
					{
						setJobFacility(f1);
						f1->NumUnits++;
					}
				}
			}
		}
		else
		{
			if (sec < 10800)
			{
				if (NumEat == 0) {
					erase();
					return;
				}
			}
			else if (sec < 21600)
			{
				NumEat = 0;
			}
			if (IsSleep) return;
		}


		//メッセージ処理用
		if (RandomBool(0.01)) textDraw = 180;
		if (textDraw > 0) textDraw--;

		if (MovingSpeed == 0)
		{
			//ジャンプアニメーション
			if (MovingTimer4 > 0)
			{
				if (MovingTimer4 <= AP)
				{
					AP -= MovingTimer4;
					MovingTimer4 = 0;
				}
				else
				{
					MovingTimer4 -= AP;
					AP = 0;
				}
			}
			else
			{
				if (Delay < 0)
				{
					Timer1 = 0;	//処理停止
					if ((Delay + AP) < 0)
					{
						Delay += AP;
						AP = 0;
					}
					else
					{
						AP += Delay;
						Delay = 0;
					}
				}
				else
				{
					if (Timer2 < Timer1)
					{
						if ((Timer1 - Timer2) <= AP)
						{
							AP -= (Timer1 - Timer2);
							Timer2 = 0;
							Timer1 = 0;
						}
						else
						{
							Timer2 += AP;
							AP = 0;
						}
					}
					else unitAI();
				}
			}
		}
		else move();

		if (IsSleep) break;
		if (AP == 0) break;
	}
}
void Unit::move()
{
	//ジャンプアニメーション
	if (MovingTimer4 <= 0) MovingTimer4 += 30;

	if (Timer2 < Timer1)
	{
		if ((Timer1 - Timer2) <= AP)
		{
			MovingTimer4 = (MovingTimer4 - (Timer1 - Timer2) + 30) % 30;
			AP -= (Timer1 - Timer2);
			Timer2 = Timer1;
		}
		else
		{
			MovingTimer4 = (MovingTimer4 - AP + 30) % 30;
			Timer2 += AP;
			AP = 0;
		}
	}
	else
	{
		if (MovingSleepMode)	//建物の中にいる場合
		{
			Moved += Timer1*Ang*MovingSpeed;
			Timer1 = 0;
			Timer2 = 0;
			MovingSpeed = 0;
			if (hasTarPos) setTarPos();	//移動処理を発生
		}
		else
		{
			//パターン移動処理
			if (MovingProgress < NumRootPoints)
			{
				Moved += Ang*RootTimer[MovingProgress];
				MovingProgress++;
				if (MovingProgress >= NumRootPoints)	//もし移動が完全に終わった場合
				{
					MovingSpeed = 0;
					Timer1 = 0;
					Timer2 = 0;
					hasTarPos = false;
					MovingSleepMode = true;
					Pos = getChip(TarPos).getCenterPos();
					Moved = Vec2(0, 0);
					return;
				}
				else
				{
					Ang = RootAng[MovingProgress];
					MovingSpeed = 1.0;
					Timer1 = (int)RootTimer[MovingProgress];
					Timer2 = 0;
				}
			}
		}
	}
}