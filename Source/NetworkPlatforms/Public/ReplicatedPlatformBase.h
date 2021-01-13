// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatedPlatformBase.generated.h"

/*
* Basic platform replicating a static mesh and its location on all clients.
* Replicated location is NOT applied in this class.
*/
UCLASS(Abstract, ClassGroup = Platforms)
class NETWORKPLATFORMS_API AReplicatedPlatformBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	// Location on the Server. On the client, it's the last known server position of the platform.
	UPROPERTY(Replicated)
	FVector ServerLocation;


public:	
	// Sets default values for this actor's properties
	AReplicatedPlatformBase();

protected:
	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CallPreReplication(UNetDriver* NetDriver);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
