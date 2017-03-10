#pragma once
#include<Siv3D.hpp>
#include"Items.h"

class Chip;
class Unit;
class Place;
class PPointer;
#define RoadColor Palette::Khaki
#define PlainsColor Palette::Green
#define ForestColor Palette::Darkgreen
#define OceanColor Palette::Blue
#define FarmColor Color(143,87,31)
#define ChipImageSize	64	//�`�b�v�摜�̑傫��
#define RoadWidth		45.2
#define ChipXMax 2048	//�`�b�v�̕ۏ؂���ő�l
#define ChipYMax 2048	//�`�b�v�̕ۏ؂���ő�l

//�O���[�o���֐�
void InitAll();
bool SetVillage(const Point& _pos);

extern double soundVolume;
extern bool setupByPOMS();
enum IMode
{
	None,
	DestroyPlaceMode,
	DestroyRoadMode,
	MakingHouseMode,
	SettingRoadMode,
	UnitFallDownMode,
	MakingFarmMode,
	DestroyFarmMode,
};
extern IMode iMode;
extern Vec2 nowMousePosAsGlobalVec2;
extern Vec2 previousMousePosAsGlobalVec2;
extern Place makingHouseP;
extern Chip* previousSelectedChip;
extern Array<Unit*> selectedUnits;
extern bool mouseOverInterface;
extern Texture mapTexture;

//�O���[�o���^�C�}
extern int sec;
extern int month;
extern int year;
extern int culturePoint;
int getHour();
int getMinute();
bool isNight();
//Interface
void UpdateInterface();	//draw�����˂�

struct PlaceData
{
	PlaceData(const int& _type)
	{
		CSVReader reader(L"Data/PlaceData.csv");
		name = reader.getOr<String>(_type, 1, String());
		size = reader.getOr<Point>(_type, 2, Point(0, 0));
		entraceY = reader.getOr<int>(_type, 3, 0);
		color = reader.getOr<Color>(_type, 4, Color(0, 0, 0, 255));
		capacityMax = reader.getOr<int>(_type, 5, 0);
		isStorage = reader.getOr<bool>(_type, 6, false);
		isWorkPlace = reader.getOr<bool>(_type, 7, false);
		jobType = reader.getOr<int>(_type, 8, 0);
		itemMax = reader.getOr<int>(_type, 9, 0);
	}
	String	name;
	Point	size;
	int		entraceY;
	Color	color;
	int		capacityMax;
	int		itemMax;
	int		jobType;
	bool	isStorage;
	bool	isWorkPlace;
};
extern Array<PlaceData> placeData;

#define PlaceMax 10000
extern Place places[PlaceMax];


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
	void	Place::drawFrame(const int& _type = 0) const;	//�����A_type==1�Ȃ�ΐԕ\��
	void	Place::drawName(const int& _type = 0) const;
	void	Place::reset();
	void	Place::set(const int& _r, const Point& _pos, const int& _t);
	int		Place::getCapacityMax() const;
	bool	Place::isStorage() const;
	bool	Place::isWorkPlace() const;
	int		Place::getJobType() const;
public:

	int		Place::capacity;
	int		Place::r;		//����
	bool	Place::enabled;
	int		Place::type;	//�����̃^�C�v
	Items	Place::items;
	Point	Place::pos;		//�����̍���̈ʒu

	int		Place::THIS;
};
bool setPlace(const int& _r, const Point& _pos, const int& _t);
extern Place* selectedPlace;
bool canSetPlace(const int& _r, const Point& _pos, const int& _t);

class PPointer
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
class Chip : public PPointer
{
public:
	void	Chip::reset();
	void	Chip::drawGround();
	void	Chip::drawRoad(const Color& _color, const double& _width);
	Chip&	Chip::getNearChip(const int& _r);
	Rect	Chip::getDrawRect() const { return Rect(THIS*ChipImageSize, Point(ChipImageSize, ChipImageSize)); }
	bool	Chip::canSetRoad() const { return isLand && (getPlace() == NULL || isRoad) && !isForest && !isFarm; }
	Vec2	Chip::getGlobalPos() const { return Vec2(THIS*ChipImageSize).movedBy(ChipImageSize / 2, ChipImageSize / 2); }
	int		Chip::getRoadLevel() const
	{
		if (!isLand) return 100;
		else if (isRoad) return 1;
		else if (isForest) return 5;
		else if (isFarm) return 5;
		else return 100;
	}
public:
	bool	Chip::isUsedByUnit;
	bool	Chip::isForest;
	bool	Chip::isFarm;
	int		Chip::growth;
	bool	Chip::isRoad;
	bool	Chip::isLand;
	int		Chip::numItem;
	int		Chip::number;	//�ėp		�ʏ펞0�ł��邱�Ƃ�ۏ؂���
	int		Chip::ang;		//�ėpAng	�ʏ펞0�ł��邱�Ƃ�ۏ؂��Ȃ�
	bool	Chip::flag;		//�ėp�t���O�ʏ펞false�ł��邱�Ƃ�ۏ؂���

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

#define Market 0

#define Laborer		0
#define Farmer		1
#define Lumberjack	2
#define Merchant	3
#define Brewer		4

#define Wood	0
#define Wheat	1
#define Sake	6


#define GoToHome		1
#define GoToForest		3
#define GoToFarm		4

#define UnitMax 10000
#define UnitRootMax 100
class Unit
{
public:
	void	Unit::erase();
	void	Unit::reset();
	bool	Unit::set(const Vec2& _globalPos);
	void	Unit::drawBody() const;
	void	Unit::update(const int& _actionPoint);
	bool	Unit::goTo(const Point& _pos);
	bool	Unit::goTo(bool(*func)(Chip*));
	bool	Unit::goTo(const int& _type);
	bool	Unit::goToTakeItem(const int& _itemID, const bool& _takeFromStrage = true);
	Vec2	Unit::getRealPos() const;
	bool	Unit::setPlace(Place* _p);
	void	Unit::unitAI();
	bool	Unit::canMove(const Point& _p)
	{
		if (getChip(_p).isRoad ||
			(jobType == Lumberjack && getChip(_p).isForest) ||
			(jobType == Farmer && getChip(_p).isFarm)) return true;
		else return false;
	}
	Place*	Unit::getPlace() { return getChip(pos).getPlace(); }
public:
	bool	Unit::needSake;
	bool	Unit::needFood;
	Point	Unit::tarPos;
	PPointer Unit::home;
	int		Unit::jobType;
	int		Unit::rootNow;
	int		Unit::rootMax;
	double	Unit::rootTimer[UnitRootMax];
	Vec2	Unit::rootAng[UnitRootMax];
	bool	Unit::enabled;
	String	Unit::message;
	Items	Unit::items;
	bool	Unit::isInPlace;
	bool	Unit::isMoving;
	double	Unit::movingSpeed;
	Vec2	Unit::movingAng;
	int		Unit::timerNow;	//�ėp�^�C�}
	int		Unit::timerMax;	//�ėp�^�C�}
	bool	Unit::workStopPenalty;	//true�̊Ԏd�����s��Ȃ��Bfalse�����͌����̕ω��Ǝ��Ԃ̕ω�
	int		Unit::progress;
	Point	Unit::pos;		//�������W	Chip�Ɋ�Â�
	Vec2	Unit::addPos;		//�ǉ����W�B����������A�ړ�����\��

	int		Unit::THIS;
};
extern Unit units[UnitMax];