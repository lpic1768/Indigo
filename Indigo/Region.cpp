#include"Header.h"

Region regions[REGION_MAX];
int	numRegions;

void Region::addChip(Chip* chip)
{
	if (!Enabled)
	{
		Enabled = true;
		BiomeType = chip->BiomeType;
	}
	if (chip->Pos.x < XMin) XMin = chip->Pos.x;
	if (chip->Pos.y < YMin) YMin = chip->Pos.y;
	if (chip->Pos.x > XMax) XMax = chip->Pos.x;
	if (chip->Pos.y > YMax) YMax = chip->Pos.y;
	NumChips++;
	SumPos += chip->getCenterPos();
}
void Region::draw()
{
	if (!Enabled) return;
	Point size((XMax - XMin + 1)*CHIP_SIZE, (YMax - YMin + 1)*CHIP_SIZE);
	Point pos(XMin*CHIP_SIZE, YMin*CHIP_SIZE);
	//Rect(pos, size).draw(Color(255, 255, 255, 64));
	FontAsset(L"font64")(L"").drawCenter((SumPos / NumChips).movedBy(4, -4), Palette::Black);
	FontAsset(L"font64")(L"").drawCenter((SumPos / NumChips), Palette::White);
}