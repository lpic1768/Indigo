#include"Header.h"
# include <HamFramework.hpp>
#include<Siv3D.hpp>

int setupProgress;
Chip* landChips[LAND_CHIPS_MAX];
int numLandChips;
Chip* regionChips[100000];
void drawChipsToImage(Image& _image)
{
	Circle(0, 0, 100).overwrite(_image, Palette::Black);
}
void drawDebugChips()
{
	double minX = (-Graphics2D::GetTransform()._31) / Graphics2D::GetTransform()._11 / CHIP_SIZE;
	double minY = (-Graphics2D::GetTransform()._32) / Graphics2D::GetTransform()._11 / CHIP_SIZE;
	double maxX = (Window::Size().x - Graphics2D::GetTransform()._31) / Graphics2D::GetTransform()._11 / CHIP_SIZE + 1;
	double maxY = (Window::Size().y - Graphics2D::GetTransform()._32) / Graphics2D::GetTransform()._11 / CHIP_SIZE + 1;
	if (minX < 0) minX = 0;
	if (minY < 0) minY = 0;
	if (maxX > CHIP_X_MAX) maxX = CHIP_X_MAX;
	if (maxY > CHIP_Y_MAX) maxY = CHIP_Y_MAX;
	if (Graphics2D::GetTransform()._11 < 2.0 / (double)CHIP_SIZE)
	{
		mapTexture.resize(CHIP_X_MAX*CHIP_SIZE, CHIP_Y_MAX*CHIP_SIZE).draw();
	}
	else
	{
		for (int x = (int)minX; x < (int)maxX; x++)
			for (int y = (int)minY; y < (int)maxY; y++)
				chips[x][y].debugDraw();
		for (int x = (int)minX; x < (int)maxX; x++)
		{
			for (int y = (int)minY; y < (int)maxY; y++)
			{
				const Chip& c = chips[x][y];
				const Point pos(c.Pos*CHIP_SIZE);
				const Point size(CHIP_SIZE, CHIP_SIZE);
				if (mouseOveredChip != NULL &&
					c.getJoinedRegion()->NumUnits != 0 &&
					mouseOveredChip->IsLand &&
					mouseOveredChip->getJoinedRegion() == c.getJoinedRegion()) Rect(pos, size).draw(Color(255, 255, 255, 32));
			}
		}
		
		drawFacilities();

		for (int x = (int)minX; x < (int)maxX; x++)
			for (int y = (int)minY; y < (int)maxY; y++)
				chips[x][y].drawRoad();
	}
}
