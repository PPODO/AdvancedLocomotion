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


};