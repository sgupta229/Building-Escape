// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing an input component"), *(GetOwner()->GetName()));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s has an input component"), *(GetOwner()->GetName()));
		//bind the input access
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	//line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//if we do then attach a physics handle
	if(ActorHit != nullptr) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab, 
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released!"));
	// PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// FVector PlayerViewPointLocation;
	// FRotator PlayerViewPointRotation;
	// GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	// 	PlayerViewPointLocation,
	// 	PlayerViewPointRotation
	// );
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	// FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
	// FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;
	//if the physics handle is attached
	// if(PhysicsHandle->GrabbedComponent) {
	// 	//move the object we are holding
	// 	PhysicsHandle->SetTargetLocation(LineTraceEnd);
	// }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s, Position: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	//set up query
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//line-tracing
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = Hit.GetActor();
	// if(ActorHit) {
	// 	UE_LOG(LogTemp, Warning, TEXT("You hit %s"), *(ActorHit->GetName()));
	// }
	return Hit;
}

