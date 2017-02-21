#include"Unit.h"
#include"Header.h"
int		Unit::receiveFromFacility(const int& id, const int& amount)
{
	if (getStandingChip().getOwnFacility() == NULL) return 0;
	Facility& facility = *getStandingChip().getOwnFacility();
	if (id == 0)
	{
		//全Itemを受け取る
		int num = 0;
		for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
		{
			if (facility.ItemsAmount[i] == 0) continue;
			addItem(facility.ItemsID[i], facility.ItemsAmount[i]);
			num += facility.ItemsAmount[i];
			facility.ItemsAmount[i] = 0;
			facility.NumItems--;
		}
		return num;
	}
	//特定アイテムのみ受け取る
	for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
	{
		if (facility.ItemsAmount[i] == 0) continue;
		if (facility.ItemsID[i] != id) continue;
		if (amount >= facility.ItemsAmount[i])
		{
			const int j = facility.ItemsAmount[i];
			facility.ItemsAmount[i] = 0;
			facility.NumItems--;
			addItem(id, j);
			return j;
		}
		else
		{
			addItem(id, amount);
			facility.ItemsAmount[i] -= amount;
			return amount;
		}
	}
	return 0;

}
void	Unit::sendItemToFacility(const int& id)	//0で全アイテム
{
	if (NumItems == 0) return;	//何も持っていない場合
	if (id == 0)
	{
		//全Itemを預ける
		for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] == 0) continue;
			getStandingChip().getOwnFacility()->addItem(ItemsID[i], ItemsAmount[i]);
			ItemsAmount[i] = 0;
			NumItems--;
		}
		return;
	}
	//特定のItemのみを預ける
	for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
	{
		if (ItemsID[i] != id || ItemsAmount[i] == 0) continue;
		getStandingChip().getOwnFacility()->addItem(ItemsID[i], ItemsAmount[i]);
		ItemsAmount[i] = 0;
		NumItems--;
		return;
	}
}
void	Region::addItem(const int& id, const int& amount)
{
	if (NumItems > 0)
	{
		for (int i = 0; i < UNIT_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] > 0)
			{
				if (ItemsID[i] == id)
				{
					ItemsAmount[i] += amount; //idがすでに持っているものにある場合
					if (ItemsAmount[i] == 0) NumItems--;
					return;
				}
			}
		}
	}
	if (amount <= 0) return;	//引く場合
								//無い場合、新規作成
	for (int i = 0; i < UNIT_ITEMS_MAX; i++)
	{
		if (ItemsAmount[i] == 0)
		{
			ItemsID[i] = id;
			ItemsAmount[i] = amount;
			NumItems++;
			return;
		}
	}
}
void	Unit::produceItem(const int& id, const int& amount)
{
	addItem(id, amount);
	getJoinedRegion()->addItem(id, amount);
}
void	Unit::addItem(const int& id, const int& amount)
{
	if (NumItems > 0)
	{
		for (int i = 0; i < UNIT_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] > 0)
			{
				if (ItemsID[i] == id)
				{
					ItemsAmount[i] += amount; //idがすでに持っているものにある場合
					if (ItemsAmount[i] == 0) NumItems--;
					return;
				}
			}
		}
	}
	if (amount <= 0) return;	//引く場合
	//無い場合、新規作成
	for (int i = 0; i < UNIT_ITEMS_MAX; i++)
	{
		if (ItemsAmount[i] == 0)
		{
			ItemsID[i] = id;
			ItemsAmount[i] = amount;
			NumItems++;
			return;
		}
	}
}
void	Facility::addItem(const int& id, const int& amount)
{
	if (NumItems > 0)
	{
		int count = 0;
		for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] > 0)
			{
				if (ItemsID[i] == id)
				{
					ItemsAmount[i] += amount; //idがすでに持っているものにある場合
					return;
				}
				count++;
				if (count == NumItems) break;
			}
		}
	}
	//無い場合、新規作成
	for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
	{
		if (ItemsAmount[i] == 0)
		{
			ItemsID[i] = id;
			ItemsAmount[i] = amount;
			NumItems++;
			return;
		}
	}
}
int	 Facility::getNumItem(const int& id)	//0で全アイテム数を返す
{

	if (NumItems == 0) return 0;	//何も持っていない場合

	if (id == 0)
	{
		int num = 0;
		for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] == 0) continue;
			num += ItemsAmount[i];
		}
		return num;
	}

	//特定のItemのみ
	for (int i = 0; i < FACILITY_ITEMS_MAX; i++)
		if (ItemsID[i] == id && ItemsAmount[i] != 0) return ItemsAmount[i];
	return 0;
}
int	 Unit::getNumItem(const int& id)
{
	if (NumItems == 0) return 0;	//何も持っていない場合

	if (id == 0)
	{
		int num = 0;
		for (int i = 0; i < UNIT_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] == 0) continue;
			num += ItemsAmount[i];
		}
		return num;
	}

	//特定のItemのみ
	for (int i = 0; i < UNIT_ITEMS_MAX; i++)
		if (ItemsID[i] == id && ItemsAmount[i] != 0) return ItemsAmount[i];
	return 0;
}