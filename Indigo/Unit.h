#pragma once
#include"Header.h"


#define UNIT_ITEMS_MAX 10
#define UNIT_ROOT_MAX 512

enum GenderType
{
	Male,
	Female
};
enum JobType
{
	Labor,
	Builder,
	Farmer,
	Woodcutter,
	Brewer,
	Merchant,
};
enum Pattern
{
	DoNothing,
	IsSleeping,
	TakeMeal,
	GoToSleep,
	WorkAsBuilder,
	WorkAsFarmer,
	WorkAsWoodcutter,
	WorkAsBrewer,
	Deliver,

};

#define Wood 1
#define Sake 7
#define Rice 8

struct Unit
{
	String	Unit::getMessage();
	void	Unit::erase();
	void	Unit::set(Facility* _f);
	void	Unit::update();
	void	Unit::draw() const;
	void	Unit::drawB();
	void	Unit::reset();
	void	Unit::move();
	void	Unit::setTarPos();
	Chip&	Unit::getStandingChip();
	void	Unit::addItem(const int& id, const int& amount);
	void	Unit::sendItemToFacility(const int& id = 0);	//0で全アイテム
	int		Unit::getNumItem(const int& id = 0);	//0で全アイテム数を返す
	void	Unit::goEntrance();
	void	Unit::goRoomRandom();
	bool	Unit::goNearMarket();
	bool	Unit::goTo(Facility* _f);
	bool	Unit::goTo(Chip* _chip);
	int		Unit::receiveFromFacility(const int& id = 0, const int& amount = 1);//返り値は取れた数

	void	Unit::unitAI();

	void	Unit::setStatus(const Pattern& _status);
	void	Unit::setPenalty(const int& _penalty);
	bool	Unit::pickUpItem(const int& _id);

	//建物検索システム
	Facility*	Unit::searchPlannedSite();
	Facility*	Unit::searchItem(const int& _id);	//一番近い_idを持っている建物
	bool	Unit::isConeected(const Facility* _f);
	bool	Unit::isConeected(const Chip* _chip);
	bool	Unit::goToNearForest();
	Region*	Unit::getJoinedRegion() const;
	void	Unit::setJoinedRegion(const Region* _r);

	Facility*	Unit::getTarFacility() const;
	void		Unit::setTarFacility(const Facility* _f);
	Facility*	Unit::getHomeFacility() const;
	void		Unit::setHomeFacility(const Facility* _f);
	Facility*	Unit::getJobFacility() const;
	void		Unit::setJobFacility(const Facility* _f);
	bool	Unit::hasHomeFacility() { return getHomeFacility() != NULL; }
	bool	Unit::hasJobFacility() { return getJobFacility() != NULL; }
	bool	Unit::hasTarFacility() { return getTarFacility() != NULL; }
	void	Unit::produceItem(const int& id, const int& amount);

	JobType		Unit::getJob();

	int		aiProgress;
	int		Age;
	GenderType Gender;
	int		textDraw;
	int		TarFacilityP;
	int		HomeFacilityP;
	int		JobFacilityP;
	Point	TarPos;
	int		JoinedRegionP;
	int		JoinedFamilyP;
	int		AP;
	//移動
	bool	hasTarPos;
	bool	MovingSleepMode;
	double	MovingSpeed;
	int		Timer1;
	int		Timer2;
	int		MovingTimer4;
	int		MovingProgress;
	int		NumEat;	//一日に食べたものの量
	Vec2	Ang;
	double	RootTimer[UNIT_ROOT_MAX];
	Vec2	RootAng[UNIT_ROOT_MAX];
	int		NumRootPoints;
	bool	Enabled;
	bool	IsSleep;
	Vec2	Pos;
	Vec2	Moved;
	//ユニットAI用
	int		Delay;
	Pattern Status;
	int		Progress;

	//持ち物
	int		ItemsAmount[UNIT_ITEMS_MAX];
	int		ItemsID[UNIT_ITEMS_MAX];
	int		NumItems;
	int THIS;

};
void	chipFlagReset();
void	updateUnits();
void	drawUnits();
void	setUnit(Facility* _f);