
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BoatInterface.generated.h"


class UBoostComponent;

UINTERFACE(MinimalAPI)
class UBoatInterface : public UInterface
{
	GENERATED_BODY()
};

class BOATRACE_API IBoatInterface
{
	GENERATED_BODY()

public:
	virtual void Drive(float InputX, float InputY) = 0;
	virtual void UpdateCheckPoint(const FName& CurrentBoxOverlapTag) = 0;
	virtual void UpdateMaxCheckPoints(int32 MaxCheckPoints) = 0;
	virtual void UpdateTotalLaps(int32 LevelTotalLaps) = 0;
	virtual void RotateBoat(float InputY, float InputX) = 0;
	virtual void SetHandbrakeActive(bool bActive) = 0;
	virtual void SetBoostActive(bool bBoostActive, float RefillFuel) = 0;

};
