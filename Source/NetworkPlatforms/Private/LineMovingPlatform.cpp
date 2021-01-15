// Fill out your copyright notice in the Description page of Project Settings.


#include "LineMovingPlatform.h"
#include "DrawDebugHelpers.h"

ALineMovingPlatform::ALineMovingPlatform()
	: Super()
{

	FromLocation = FVector(000, 0, 0);
	ToLocation = FVector(100, 0, 0);
	MovementSpeed = 100.0f;

#if WITH_EDITOR
	EDITORONLY_EndLocationPreview = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EDITORONLY_EndLocationPreview"));
	EDITORONLY_EndLocationPreview->SetupAttachment(RootComponent);
	EDITORONLY_EndLocationPreview->SetIsReplicated(false);
	EDITORONLY_EndLocationPreview->SetStaticMesh(StaticMesh->GetStaticMesh());
	EDITORONLY_EndLocationPreview->SetRelativeLocationAndRotation(ToLocation, StaticMesh->GetRelativeRotation());
	EDITORONLY_EndLocationPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> WireframeMat(TEXT("/Game/Materials/M_Wireframe"));;
	if (WireframeMat.Succeeded())
	{
		EDITORONLY_EndLocationPreview->SetMaterial(0, WireframeMat.Object);
	}

	bEDITORONLY_ShowDebug = false;
#endif
}

void ALineMovingPlatform::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property) ? e.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ALineMovingPlatform, FromLocation)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(ALineMovingPlatform, ToLocation))
	{
		MovementDirection = (ToLocation - FromLocation).GetSafeNormal();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ALineMovingPlatform, FromLocation))
	{
		StaticMesh->SetRelativeLocation(FromLocation);
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ALineMovingPlatform, ToLocation))
	{
#if WITH_EDITOR
		EDITORONLY_EndLocationPreview->SetRelativeLocation(ToLocation);
#endif
	}
}

void ALineMovingPlatform::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALineMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALineMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ServerMove(DeltaTime);
	}
	else
	{
		ClientMove(DeltaTime);
	}

#if WITH_EDITOR
	if (bEDITORONLY_ShowDebug)
	{
		DrawDebugLine(
			GetWorld(),
			GetActorTransform().TransformPosition(FromLocation),
			GetActorTransform().TransformPosition(ToLocation),
			FColor::Emerald,
			false,
			0.0f,
			SDPG_World,
			1.0f
		);
	}

#endif
}

void ALineMovingPlatform::ClientMove(float DeltaTime)
{
	constexpr float InterpolationSpeed = 100.0f;
	// SetActorLocation(FMath::VInterpTo(GetActorLocation(), ServerLocation, DeltaTime, InterpolationSpeed));
	AddActorWorldOffset(GetActorLocation() - FMath::VInterpTo(GetActorLocation(), ServerLocation, DeltaTime, InterpolationSpeed));
}

void ALineMovingPlatform::ServerMove(float DeltaTime)
{
	StaticMesh->SetWorldLocation(StaticMesh->GetComponentLocation() + MovementDirection * DeltaTime * MovementSpeed);
}
