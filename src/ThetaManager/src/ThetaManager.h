// ThetaManager.h
#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "JsonUtilities.h"
#include "ThetaManager.generated.h"

UCLASS(config = Game, defaultconfig)
class THETAPLUGIN_API UThetaManagerSettings : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(Config, EditAnywhere, Category = "Theta|Settings")
    FString ApiUrl = "DEFAULT_API_URL"; // Set a default value

    UFUNCTION(BlueprintCallable, Category = "Theta|Settings")
    static FString GetApiUrl();
};

UCLASS()
class THETAPLUGIN_API UThetaManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Theta|Camera")
    static float GetCameraTemperature();
};