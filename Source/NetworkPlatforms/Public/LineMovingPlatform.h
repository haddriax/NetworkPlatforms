// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicatedPlatformBase.h"
#include "LineMovingPlatform.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, AutoExpandCategories = ("Plateform Movement") )
class NETWORKPLATFORMS_API ALineMovingPlatform : public AReplicatedPlatformBase
{
	GENERATED_BODY()
	
protected:
#if WITH_EDITOR
	UPROPERTY(VisibleAnywhere, Category = "Editor Helpers")
	UStaticMeshComponent* EDITORONLY_EndLocationPreview;

	UPROPERTY(EditAnywhere, Category = "Helpers")
	uint8 bEDITORONLY_ShowDebug	;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true), Category="Plateform Movement")
	FVector FromLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true), Category = "Plateform Movement")
	FVector ToLocation;

	UPROPERTY(VisibleAnywhere, Category = "Plateform Movement")
	bool bReverse;

	UPROPERTY(VisibleAnywhere, Category = "Plateform Movement")
	FVector MovementDirection;

	UPROPERTY(EditAnywhere, Category = "Plateform Movement")
	float MovementSpeed;
public:
	ALineMovingPlatform();

protected:
	void PostEditChangeProperty(FPropertyChangedEvent& e) override;

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void ClientMove(float DeltaTime);

	void ServerMove(float DeltaTime);

};
