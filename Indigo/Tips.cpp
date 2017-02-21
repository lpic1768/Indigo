#include"Header.h"

void closeAllInterfaceMode()
{
	constructingModeEnabled = false;
	destroyerModeEnabled = false;
	fallingDownModeEnabled = false;
	makingRoadModeEnabled = false;
	facilityDestroyerModeEnabled = false;
	roadDestroyerModeEnabled = false;
	roadStartChip = NULL;
	closeTwitterGUI();
}