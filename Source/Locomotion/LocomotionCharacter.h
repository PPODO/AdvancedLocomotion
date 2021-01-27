#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Locomotion/AdvancedLocomotionAnimInstance.h>
#include <Curves/CurveVector.h>
#include "LocomotionCharacter.generated.h"

USTRUCT(BlueprintType)
struct FFMovementSetting {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		float WalkSpeed;
	UPROPERTY(BlueprintReadWrite)
		float RunSpeed;
	UPROPERTY(BlueprintReadWrite)
		float SprintSpeed;

	UPROPERTY(BlueprintReadWrite)
		class UCurveVector* MovementCurve;
	UPROPERTY(BlueprintReadWrite)
		class UCurveFloat* RotationRateCurve;

public:
	FFMovementSetting() : WalkSpeed(0.f), RunSpeed(0.f), SprintSpeed(0.f), RotationRateCurve(nullptr), MovementCurve(nullptr) {};
	FFMovementSetting(float fWalkSpeed, float fRunSpeed, float fSprintSpeed, class UCurveFloat* rotationCurve, class UCurveVector* movementCurve) 
		: WalkSpeed(fWalkSpeed), RunSpeed(fRunSpeed), SprintSpeed(fSprintSpeed), RotationRateCurve(rotationCurve), MovementCurve(movementCurve) {};
	FFMovementSetting(float fWalkSpeed, float fRunSpeed, float fSprintSpeed, FString rotationCurveAssetPath, FString movementCurveAssetPath)
		: WalkSpeed(fWalkSpeed), RunSpeed(fRunSpeed), SprintSpeed(fSprintSpeed) {
		ConstructorHelpers::FObjectFinder<class UCurveFloat> rotationCurveFloat(*rotationCurveAssetPath);
		ConstructorHelpers::FObjectFinder<class UCurveVector> movementCurveVector(*movementCurveAssetPath);

		if (rotationCurveFloat.Succeeded())
			RotationRateCurve = rotationCurveFloat.Object;
		if (movementCurveVector.Succeeded())
			MovementCurve = movementCurveVector.Object;
	};

};

UCLASS()
class LOCOMOTION_API ALocomotionCharacter : public ACharacter {
	GENERATED_BODY()

public:
	ALocomotionCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void MoveRight(float value);

private:
	void PlayerMovementInput(bool bIsForwardInput);

#pragma region essential information
private:
	void SetEssentialValues();

	FVector CalculateAcceleration();

public:
	void GetEssentialValues(FVector& vVelocity, FVector& vAcceleration, FVector& vMovementInput, bool& bIsMoving, bool& bHasMovementInput, float& fSpeed, FRotator& rAimRotation, float& fAimYawRate);
	
private:
	FVector m_vAcceleration;
	
	bool m_bIsMoving;
	bool m_bHasMovementInput;

	float m_fSpeed;
	float m_fMovementInputAmount;
	float m_fAimYawRate;

	FRotator m_rLastVelocityRotation;
	FRotator m_rLastMovementInputRotation;

#pragma endregion
#pragma region cache values
private:
	void CacheValues();

private:
	FVector m_vPreviousVelocity;
	float m_fPreviousAimYaw;

#pragma endregion
#pragma region movement system
private:
	void UpdateCharacterMovement();
	void UpdateDynamicMovementSetting(ECharacterGait allowedGait);

	bool CanSprint();

	FFMovementSetting GetTargetMovementSetting();

private:
	FFMovementSetting m_currentMovementSetting;

#pragma region state values
private:
	ECharacterGait GetAllowedGait();
	ECharacterGait GetActualGait(ECharacterGait allowedGait);

private:
	ERotationMode m_rotationMode;
	ECharacterGait m_gait;

#pragma endregion
#pragma endregion
#pragma region input
private:
	ECharacterGait m_desiredGait;

#pragma endregion


};