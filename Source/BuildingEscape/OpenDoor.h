// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Pawn.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();

private:
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 90.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	//a pawn is a type of actor so we can do this
	UPROPERTY(VisibleAnywhere)
	AActor* ActorThatOpens;

};
