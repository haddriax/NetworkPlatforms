// Fill out your copyright notice in the Description page of Project Settings.

#include "ReplicatedPlatformBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AReplicatedPlatformBase::AReplicatedPlatformBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Movable);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetIsReplicated(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultStaticMesh(TEXT("/Game/Geometry/Meshes/1M_Cube_Chamfer"));
	if (DefaultStaticMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultStaticMesh.Object);
	}

	StaticMesh->SetMobility(EComponentMobility::Movable);
	bReplicates = true;

}

void AReplicatedPlatformBase::PostInitializeComponents()
{

	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AReplicatedPlatformBase::BeginPlay()
{
	Super::BeginPlay();
}

void AReplicatedPlatformBase::CallPreReplication(UNetDriver* NetDriver)
{
	Super::CallPreReplication(NetDriver);

	ServerLocation = GetActorLocation();
}

// Called every frame
void AReplicatedPlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ServerLocation = GetActorLocation();
	}
}

void AReplicatedPlatformBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// DOREPLIFETIME(AReplicatedPlatformBase, ServerLocation);
	DOREPLIFETIME_CONDITION(AReplicatedPlatformBase, ServerLocation, COND_SkipOwner);
}