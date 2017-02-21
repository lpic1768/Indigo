#include"Unit.h"
#include"Header.h"

bool saveAll()
{
	if (const auto path = Dialog::GetSaveAll())
	{
		BinaryWriter writer(path.value());

		//ŠÔ‚Ì‘‚«‚İ
		writer.write(year);
		writer.write(season);
		writer.write(sec);

		writer.write(downloadMap);
		writer.write(chips);

		writer.write(numUnits);
		writer.write(units);

		writer.write(numZones);
		writer.write(zones);

		writer.write(numFacilities);
		writer.write(facilities);


		return true;
	}
	return false;
}

bool loadAll()
{
	if (const auto path = Dialog::GetOpenAll())
	{
		allReset();
		BinaryReader reader(path.value());
		//ŠÔ‚Ì“Ç‚İ‚İ
		reader.read(year);
		reader.read(season);
		reader.read(sec);

		reader.read(downloadMap);
		reader.read(chips);

		reader.read(numUnits);
		reader.read(units);

		reader.read(numZones);
		reader.read(zones);


		reader.read(numFacilities);
		reader.read(facilities);


		updateMapImage();
		return true;
	}
	return false;
}