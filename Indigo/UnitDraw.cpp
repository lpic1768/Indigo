#include"Unit.h"
#include "Header.h"
void Unit::draw() const
{
	//Root‚Ì•\Ž¦
	if (!MovingSleepMode)
	{
		Vec2 pos = Pos + Moved + RootTimer[MovingProgress] * RootAng[MovingProgress];
		Line(Pos.movedBy(Moved) + Ang*MovingSpeed*Timer2, pos).draw(3, Color(255, 255, 255, 64));
		for (int i = MovingProgress + 1; i < NumRootPoints; i++)
		{
			Line(pos, pos + RootTimer[i] * RootAng[i]).draw(3, Color(255, 255, 255, 64));
			pos += RootTimer[i] * RootAng[i];
		}
	}
}
void Unit::drawB()
{
	Vec2 pos = Pos.movedBy(Moved) + Ang*MovingSpeed*Timer2 + Vec2(0, -5) + 24 * Vec2(0, -Abs(Sin(double(MovingTimer4) / 30.0*Pi)));
	Circle(Pos.movedBy(Moved) + Ang*MovingSpeed*Timer2, 4.0).draw(Color(0, 0, 0, 64));

	Color color = HSV(getJob() * 36);

	double radius = 5.5;

	switch (Gender)
	{
	case Male:	color = Palette::Blue;	break;
	case Female:color = Palette::Red;	break;
	}


	if (Delay < 0) color = Palette::Black;
	Circle(pos, radius).draw(color).drawFrame(0.5, 0.5, Palette::Black);
	const Vec2 pos2 = pos.movedBy(-5, -6);

//	FontAsset(L"font8")(getJoinedFamily()->THIS, L"_", getJoinedFamily()->NumUnit, getJoinedFamily()->HasAdultFemale, getJoinedFamily()->HasAdultMale).draw((pos).movedBy(8, -8), Palette::White);

	//Item‚Ì•`‰æ
	int j = 0;
	for (int i = 0; i < UNIT_ITEMS_MAX; i++)
	{
		if (ItemsAmount[i] == 0) continue;
		j++;
		const Vec2 posI = Pos.movedBy(Moved) + Ang*MovingSpeed*Timer2 + Vec2(0, -4 - j * 8) + (24 + j * 4) * Vec2(0, -Abs(Sin(double(MovingTimer4) / 30.0*Pi)));

		TextureAsset(L"items")(16 * (ItemsID[i] - 1), 0, 16, 16).drawAt(posI);
		FontAsset(L"font8")(L"x", ItemsAmount[i]).draw((posI).movedBy(8, -8), Palette::White);

	}
	if (lineEnabled)
	{
		if (hasHomeFacility()) Line(pos, getHomeFacility()->getEntranceChip()->getCenterPos()).draw(Palette::Blue);
		if (hasJobFacility()) Line(pos, getJobFacility()->getEntranceChip()->getCenterPos()).draw(Palette::Red);
	}
	if (Timer1 > 0) Line(pos.movedBy(-6, 5), pos.movedBy(-6, 5 - Timer2 * 10 / Timer1)).draw();
}