#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LocomotionCharacter.generated.h"

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
	void GetEssentialValues(const FVector& vVelocity, const FVector& vAcceleration, const FVector& vMovementInput, bool bIsMoving, bool bHasMovementInput, float fSpeed, const FRotator& rAimRotation, float fAimYawRate);
	
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
#pragma region state values

#pragma endregion


};