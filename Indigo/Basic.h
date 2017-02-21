#pragma once
#include<Siv3D.hpp>
#include"Items.h"

class Chip;
class Unit;
class Place;
class PlacePointer;

#define RoadColor Palette::Khaki
#define PlainsColor Palette::Green
#define OceanColor Palette::Blue
#define ChipImageSize	64	//チップ画像の大きさ
#define ChipXMax 2048	//チップの保証する最大値
#define ChipYMax 2048	//チップの保証する最大値

//グローバル関数
void drawAll();
void InitAll();
bool SetVillage(const Point& _pos);
extern bool destroyPlaceMode;
extern bool destroyRoadMode;
extern bool makingHouseMode;
extern bool settingRoadMode;
extern Place makingHouseP;
extern Chip* previousSelectedChip;
extern bool mouseOverInterface;
extern Texture mapTexture;

//グローバルタイマ
extern double sec;

//Interface
void UpdateInterface();	//drawも兼ねる

#define PlaceMax 10000
extern Place places[PlaceMax];
enum PType
{
	House,
	Market,
	Farm,
};
class Place : Items
{
public:
	Place() { reset(); }
	void	Place::erase();
	Color	Place::getDrawColor() const;
	String	Place::getNameAsString() const;
	Point	Place::getSize() const;
	Point	Place::getEntrancePos() const;
	Rect	Place::getDrawRect() const { return Rect(pos*ChipImageSize, getSize()*ChipImageSize); }
	void	Place::drawFloor(const int& _type = 0) const;	//もし、_type==1ならば赤表示
	void	Place::drawName() const;
	void	Place::reset();
	void	Place::set(const int& _r, const Point& _pos, const PType& _t);

public:

	int		Place::r;		//向き
	bool	Place::enabled;
	PType	Place::type;	//建物のタイプ
	Items	Place::item;
	Point	Place::pos;		//建物の左上の位置

	int		Place::THIS;
};
bool setPlace(const int& _r, const Point& _pos, const PType& _t);
extern Place* selectedPlace;
bool canSetPlace(const int& _r, const Point& _pos, const PType& _t);

class PlacePointer
{
public:
	Place* getPlace() const
	{
		if (place == 0) return NULL;
		else return &places[place - 1];
	}
	void setPlace(const Place* _r)
	{
		if (_r == NULL) place = 0;
		else place = _r->THIS + 1;
	}
protected:
	int	place;
};

extern Chip* temp[1000000];
extern Chip chips[ChipXMax][ChipYMax];
extern int chipX;
extern int chipY;
extern Chip* nowSelectedChip;
extern Chip* newnowSelectedChip;
class Chip : public PlacePointer
{
public:
	void	Chip::reset();
	void	Chip::drawGround();	//地面の描画
	Chip&	Chip::getNearChip(const int& _r);
	Rect	Chip::getDrawRect() const { return Rect(THIS*ChipImageSize, Point(ChipImageSize, ChipImageSize)); }
	bool	Chip::canSetRoad() const { return isLand && getPlace() == NULL; }

public:
	bool	Chip::isRoad;
	bool	Chip::isLand;

	int		Chip::number;	//汎用		通常時0であることを保証する
	int		Chip::ang;		//汎用Ang	通常時0であることを保証しない
	bool	Chip::flag;		//汎用フラグ通常時falseであることを保証する

	Point	Chip::THIS;
};
Chip& getChip(const Point& _pos);
Chip& getChip(const int& _x, const int& _y);
extern Image mapImage;
bool isConnectedByLand(const Point& _posA, const Point& _posB);
bool isConnectedByRoad(const Point& _posA, const Point& _posB);
void setRoadAToB(const Point& _posA, const Point& _posB);
void drawPlannedRoadAToB(const Point& _posA, const Point& _posB);
void resetTemp();

#define UNIT_MAX 10000
class Unit
{
public:
	void reset();

	Items	items;
	int		timerNow;	//汎用タイマ
	int		timerMax;	//汎用タイマ
	Point	pos;		//内部座標	Chipに基づく
	Vec2	addPos;		//追加座標。建物内部や、移動中を表す

	int		THIS;
};
extern Unit units[UNIT_MAX];