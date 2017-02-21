#pragma once
# include <Siv3D.hpp>
# include <HamFramework.hpp>

struct Zone;
struct Chip;
struct Facility;
struct Unit;
struct Family;
struct Region;

extern Image* scrCapImageP;
extern int fillter;
extern Image mapImage;
extern Texture mapTexture;
#define ACHIEVEMENTS_MAX 10
extern int achievements[ACHIEVEMENTS_MAX];

#define CHIP_X_MAX 2048
#define CHIP_Y_MAX 2048
#define CHIP_SIZE 64

extern Chip* temp[1000000];
void updateMapImage(const int& xMin = 0, const int& yMin = 0, const int& xMax = CHIP_X_MAX, const int& yMax = CHIP_Y_MAX);
bool saveAll();
bool loadAll();
bool setupByPOMS();
void allReset();
void drawDebugChips();
void drawChipsToImage(Image& _image);
bool isNight();

void drawInterface();
void constructingMode();
void destroyerMode();
void fallingDownMode();
void makingRoadMode();

void updateAchievement();
extern Unit* cameraTarUnit;
extern int sec;
extern int season;
extern int year;
extern int setupProgress;
extern Camera2D* camera;
extern bool nightEnabled;
extern bool lineEnabled;
extern Chip* roadStartChip;
extern int downloadMap;

//GUIInterface
extern bool constructingModeEnabled;
extern bool destroyerModeEnabled;
extern bool fallingDownModeEnabled;
extern bool makingRoadModeEnabled;
extern bool	roadDestroyerModeEnabled;
extern bool	facilityDestroyerModeEnabled;
extern double soundEffectsVolume;

//GUI
extern WidgetStyle* widgetStyleP;
void closeAllInterfaceMode();

//ツイッター用
void closeTwitterGUI();
void twitterUpdate();
void twitterInit();
extern TwitterClient* twitterP;
extern bool twitterAuthEnabled;
extern bool twitterGuiEnabled;
extern String twitterText1;
extern String twitterText2;
extern GUI* guiAuthP;
extern GUI* guiTweetP;
extern int twitterCameraFlash;


extern Chip* mouseOveredChip;

enum Biome
{
	Ocean,
	Plains,
	Desert,
	Forest,
	Mountain
};

#define REGION_MAX 10000
#define LOG_ITEMS_MAX 10
struct Region
{
	void	Region::reset();
	void	Region::draw();
	void	Region::addChip(Chip* chip);
	void	Region::addItem(const int& id, const int& amount);


	int		ItemsAmount[LOG_ITEMS_MAX];
	int		ItemsID[LOG_ITEMS_MAX];
	int		NumItems;
	int		ItemsAmount2[LOG_ITEMS_MAX];
	int		ItemsID2[LOG_ITEMS_MAX];

	int		NumBuilder;
	int		NumOwnFaclities;
	int		NumHouseCap;
	int		NumJobCap;
	int		NumUnits;
	bool	Enabled;
	Biome	BiomeType;
	int		XMin;
	int		XMax;
	int		YMin;
	int		YMax;
	Vec2	SumPos;
	int		NumChips;
	int THIS;
};
extern Region regions[REGION_MAX];
extern int	numRegions;
void	setVillage(Chip* _chip);

#define ZONE_MAX 200
struct Zone
{
	void	Zone::reset();

	bool	Enabled;
	int		NumOwnChips;
	int		XMin, YMin, XMax, YMax;
	int		numOwnRegions;
	bool	hasForest;
	int THIS;
};
void drawZones();
extern Zone zones[ZONE_MAX];
extern int	numZones;


#define LAND_CHIPS_MAX CHIP_X_MAX*CHIP_Y_MAX
struct Chip
{
	void	Chip::drawRoad();
	void	Chip::reset(const int& _x, const int& _y);
	void	Chip::debugDraw();
	Vec2	Chip::getCenterPos();
	Chip&	Chip::getNearChip(const int& _value, const bool& _flag = false);
	bool	Chip::canGetNearChip(const int& _value);
	Chip&	Chip::movedBy(const int& _x, const int& _y);
	Chip&	Chip::movedBy(const Point& _pos);
	bool	Chip::isConeected(const Chip* _chip);

	//軽いシステム
	bool	Chip::isFMarket();	//立っている場所がマーケットかどうか

	Zone*	Chip::getJoinedZone() const;
	void	Chip::setJoinedZone(const Zone* _z);
	Region* Chip::getJoinedRegion() const;
	void	Chip::setJoinedRegion(const Region* _r);
	Facility* Chip::getOwnFacility() const;
	void	Chip::setOwnFacility(const Facility* _f);

	Biome	BiomeType;
	int		JoinedRegionP;
	int		JoinedZoneP;
	int		RootNumber;
	int		RoadLevel;
	int		RoadAng;
	int		Number;
	bool	IsRoad;	//上を移動可能かどうか
	bool	Flag;
	bool	IsLand;
	bool	IsInLand;
	int		OwnFacilityP;
	Point	Pos;
};
extern int actionPoint;
Chip& getChip(const Point& _pos);
Chip& getChip(const int& _i);
Chip& getChip(const Vec2& _pos);
void	setRoad(Chip* _pos1, Chip* _pos2, const bool _roadLevelFlag = true);
Vec2	getRoadPos(const Vec2& _pos1, const Vec2& _pos2);
extern Chip chips[CHIP_X_MAX][CHIP_Y_MAX];
extern Chip* landChips[LAND_CHIPS_MAX];
extern int numLandChips;
extern Chip* rootChips[1000000];


enum FacilityType
{
	House,
	Farm,
	TreeHouse,
	Market,
	CarpentersHut,
	Brewery,

};
#define FACILITY_MAX 10000
#define FACILITY_ITEMS_MAX 10
struct Facility
{
	void	Facility::erase();
	void	Facility::draw() const;
	void	Facility::drawB() const;
	void	Facility::reset();
	void	Facility::addItem(const int& id, const int& amount);
	bool	Facility::build(const int& _r, Chip* _chip, const FacilityType& _type);
	void	Facility::setRoadToForest();

	bool	Facility::IsHomeFacility();
	bool	Facility::IsJobFacility();
	bool	Facility::IsStockFacility();
	int		Facility::getNumItem(const int& id = 0);	//0で全アイテム数を返す

	Chip*	Facility::getTopLeftChip() const;
	void	Facility::setTopLeftChip(const Chip* _chip);
	Chip*	Facility::getEntranceChip() const;
	void	Facility::setEntranceChip(const Chip* _chip);


	Zone*	Facility::getJoinedZone() const;
	void	Facility::setJoinedZone(const Zone* _z);
	void	Facility::setJoinedRegion(const Region* _r);
	Region* Facility::getJoinedRegion() const;

	bool	Facility::canGetItem(const int& _id);	//備蓄も考慮
	bool	Facility::canSendItem(const int& _id);	//キャパシティーを考慮

	int		NumCrop;
	int		NumCropMax;
	int		JoinedZoneP;
	int		JoinedRegionP;

	int		R;
	Point	Size;
	int		NumUnitsMax;
	int		NumUnits;
	bool	Enabled;


	Point	TopLeftChipP;
	Point	EntranceChipP;

	double	BuildProgress;	//建造度
	FacilityType	Type;

	//持ち物
	int		ItemsAmount[FACILITY_ITEMS_MAX];
	int		ItemsID[FACILITY_ITEMS_MAX];
	int		NumItems;
	int THIS;
};
extern Facility facilities[FACILITY_MAX];
extern int		numFacilities;
void drawFacilities();
void drawFacilitiesB();
Facility* setFacility(const int& _r, Chip* _chip, const FacilityType& _type);
bool canBuild(Chip* _chip, const int& _r, const FacilityType& _type);
Point getFacilitySize(const int& _r, const FacilityType& _type);
Point getEntrancePos(const int& _r, const FacilityType& _type);
Facility* setFacilityNearChip(Chip* _chip, const FacilityType& _type);


#define UNIT_MAX 10000
extern Unit		units[UNIT_MAX];
extern int		numUnits;