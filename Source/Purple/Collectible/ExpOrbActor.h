// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExpOrbActor.generated.h"

UCLASS()
class PURPLE_API AExpOrbActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExpOrbActor();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    void Init(float InExpValue, AActor* TargetPlayer);

protected:
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void ApplyOrbColorByExp();

protected:
    UPROPERTY(VisibleAnywhere)
     TObjectPtr<class USphereComponent> Collision;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UStaticMeshComponent> Mesh;

    UPROPERTY()
    TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterial;

    FVector StartLocation;
    float AccumulatedTime = 0.0f;
    float ExpValue = 0.0f;
    bool bIsAbsorbing = false;

    UPROPERTY()
    TObjectPtr<AActor> TargetToFollow = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    float FloatAmplitude = 20.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    float FloatFrequency = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float AbsorbDistance = 500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float AbsorbSpeed = 500.0f;

};
