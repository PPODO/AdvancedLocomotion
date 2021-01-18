#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AdvancedLocomotionAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FFVelocityBlend {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Velocity Blend")
		float Velocity_F;
	UPROPERTY(EditAnywhere, Category = "Velocity Blend")
		float Velocity_B;
	UPROPERTY(EditAnywhere, Category = "Velocity Blend")
		float Velocity_L;
	UPROPERTY(EditAnywhere, Category = "Velocity Blend")
		float Velocity_R;

public:
	FFVelocityBlend() : Velocity_F(), Velocity_B(), Velocity_L(), Velocity_R() {};
	FFVelocityBlend(float f, float b, float l, float r) : Velocity_F(f), Velocity_B(b), Velocity_L(l), Velocity_R(r) {};
	
};

USTRUCT(BlueprintType)
struct FFLeanAmount {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Lean Amount")
		float Lean_X;
	UPROPERTY(EditAnywhere, Category = "Lean Amount")
		float Lean_Y;

public:
	FFLeanAmount() : Lean_X(), Lean_Y() {};
	FFLeanAmount(float x, float y) : Lean_X(x), Lean_Y(y) {};
	
};

UCLASS()
class LOCOMOTION_API UAdvancedLocomotionAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	UAdvancedLocomotionAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaSeconds) override;

private:
	class ALocomotionCharacter* m_characterInstance;

#pragma region character information
private:
	FVector m_vVelocity;
	FVector m_vAcceleration;
#pragma endregion

#pragma region value update while moving on the ground
private:
	void UpdateMovementValues(float fDeltaTime);
	void UpdateRotationValues();

private:
	const float m_fVelocityBlendInterpSpeed = 12.f;
	const float m_fLeanAmountInterpSpeed = 4.f;
	float m_fDiagonalScaleAmount;

#pragma region velocity blend
private:
	FFVelocityBlend CalculateVelocityBlend();
	FFVelocityBlend InterpVelocityBlend(const FFVelocityBlend& current, const FFVelocityBlend& target, float fInterpSpeed, float fDeltaTime);

private:
	FFVelocityBlend m_velocityBlend;

#pragma endregion
#pragma region diagonal scale amount
private:
	float CalculateDiagonalScaleAmount();

private:
	UCurveFloat* m_diagonalFloatCurve;

#pragma endregion
#pragma region acceleration/deacceleration, Lean amount
private:
	FFLeanAmount InterpLeanAmount(const FFLeanAmount& current, const FFLeanAmount& target, float fInterpSpeed, float fDeltaTime);
	FVector CalculateRelativeAccelerationAmount();

private:
	FVector m_vRelativeAccelerationAmount;
	FFLeanAmount m_leanAmount;

#pragma endregion
#pragma endregion

};