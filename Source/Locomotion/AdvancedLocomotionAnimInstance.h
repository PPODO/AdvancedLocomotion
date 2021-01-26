#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AdvancedLocomotionAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ECharacterGait : uint8 {
	Gait_Walking,
	Gait_Runing,
	Gait_Sprinting,
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8 {
	Movement_Forward,
	Movement_Backward,
	Movement_Right,
	Movement_Left
};

UENUM(BlueprintType)
enum class ERotationMode : uint8 {
	Rotation_Looking,
	Rotation_Aim
};

UENUM(BlueprintType)
enum class ETurnAngle : uint8 {
	Left_90,
	Right_90,
	Left_180,
	Right_180
};

USTRUCT(BlueprintType)
struct FFVelocityBlend {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Velocity Blend")
		float Velocity_F;
	UPROPERTY(BlueprintReadWrite, Category = "Velocity Blend")
		float Velocity_B;
	UPROPERTY(BlueprintReadWrite, Category = "Velocity Blend")
		float Velocity_L;
	UPROPERTY(BlueprintReadWrite, Category = "Velocity Blend")
		float Velocity_R;

public:
	FFVelocityBlend() : Velocity_F(), Velocity_B(), Velocity_L(), Velocity_R() {};
	FFVelocityBlend(float f, float b, float l, float r) : Velocity_F(f), Velocity_B(b), Velocity_L(l), Velocity_R(r) {};
	
};

USTRUCT(BlueprintType)
struct FFLeanAmount {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Lean Amount")
		float Lean_X;
	UPROPERTY(BlueprintReadWrite, Category = "Lean Amount")
		float Lean_Y;

public:
	FFLeanAmount() : Lean_X(), Lean_Y() {};
	FFLeanAmount(float x, float y) : Lean_X(x), Lean_Y(y) {};
	
};

USTRUCT(BlueprintType)
struct FFTurnInPlaceAsset {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place")
		class UAnimSequence* Animation;
	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place")
		ETurnAngle TurnAngle;
	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place")
		float AnimatedAngle;
	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place")
		float PlayRate;

public:
	FFTurnInPlaceAsset() : Animation(), AnimatedAngle(), PlayRate() {};
	FFTurnInPlaceAsset(class UAnimSequence* animation, ETurnAngle turnAngle, float animatedAngle, float playRate)
		: Animation(animation), AnimatedAngle(animatedAngle), TurnAngle(turnAngle), PlayRate(playRate) {};
	FFTurnInPlaceAsset(const FString& animationAssetPath, ETurnAngle turnAngle, float animatedAngle, float playRate)
		: Animation(nullptr), AnimatedAngle(animatedAngle), TurnAngle(turnAngle), PlayRate(playRate) {
		ConstructorHelpers::FObjectFinder<class UAnimSequence> animSequence(*animationAssetPath);
		if (animSequence.Succeeded())
			Animation = animSequence.Object;
	};

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
	float GetAnimCurveCompact(const FName& curveName);
	float GetAnimCurveClamp(const FName& curveName, float fBias, float fMin, float fMax);

private:
	class ALocomotionCharacter* m_characterInstance;

#pragma region character information
private:
	void UpdateCharacterInfo();
	void UpdateAimingValues();
	void UpdateFootIK();

private:
	FVector m_vVelocity;
	FVector m_vAcceleration;
	FVector m_vMovementInput;

	bool m_bIsMoving;
	bool m_bHasMovementInput;

	float m_fSpeed;
	float m_fYawRate;

	FRotator m_rAimingRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Character Information", meta = (AllowPrivateAccess = "true"))
		ECharacterGait m_gait;
	ERotationMode m_rotationMode;

#pragma endregion
#pragma region grounded
private:
	void UpdateGrounded(float fDeltaTime);
	bool ShouldMoveCheck();
	bool CanRotateInPlace();
	bool CanTurnInPlace();
	bool CanDynamicTransition();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		EMovementDirection m_movementDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		bool m_bShouldMove;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		bool m_bRotateLeft;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		bool m_bRotateRight;

	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		float m_fRotateRate;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", meta = (AllowPrivateAccess = "true"))
		float m_fRotationScale;
	float m_fElapsedDelayTime;

#pragma region do while moving
private:
	void UpdateMovementValues(float fDeltaTime);
	void UpdateRotationValues();

private:
	const float m_fVelocityBlendInterpSpeed = 12.f;
	const float m_fLeanAmountInterpSpeed = 4.f;

#pragma region velocity blend
private:
	FFVelocityBlend CalculateVelocityBlend();
	FFVelocityBlend InterpVelocityBlend(const FFVelocityBlend& current, const FFVelocityBlend& target, float fInterpSpeed, float fDeltaTime);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		FFVelocityBlend m_velocityBlend;

#pragma endregion
#pragma region diagonal scale amount
private:
	float CalculateDiagonalScaleAmount();

private:
	UCurveFloat* m_diagonalCurveFloat;

	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		float m_fDiagonalScaleAmount;

#pragma endregion
#pragma region acceleration/deacceleration, Lean amount
private:
	FFLeanAmount InterpLeanAmount(const FFLeanAmount& current, const FFLeanAmount& target, float fInterpSpeed, float fDeltaTime);
	FVector CalculateRelativeAccelerationAmount();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		FVector m_vRelativeAccelerationAmount;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		FFLeanAmount m_leanAmount;

#pragma endregion
#pragma region walk/run blend, stride blend, standing rate
private:
	float CalculateWalkRunBlend();
	float CalculateStrideBlend();
	float CalculateStandingPlayRate();

private:
	UCurveFloat* m_strideWalkCurveFloat;
	UCurveFloat* m_strideRunCurveFloat;

	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		float m_fWalkBlend;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		float m_fStrideBlend;
	UPROPERTY(BlueprintReadOnly, Category = "Grounded", Meta = (AllowPrivateAccess = "true"))
		float m_fStandingPlayRate;

#pragma endregion
#pragma region set the movement direction
private:
	EMovementDirection CalculateMovementDirection();
	EMovementDirection CalculateQuadrant(float fFLThreshold, float fFRThreshold, float fBLThreshold, float fBRThreshold, float fBuffer, float fAngle);
	bool AngleInRange(float fAngle, float fMin, float fMax, float fBuffer);

#pragma endregion
#pragma endregion
#pragma region do whiile not moving
private:
	void RotateInPlaceCheck();
	void TurnInPlaceCheck(float fDeltaTime);
	void DynamicTransitionCheck();

	void TurnInPlace(const FRotator & rTargetRotation, float fPlayRateScale, float fStartTime);

private:
	FFTurnInPlaceAsset m_CurrentTurnInPlaceAsset;
	TArray<FFTurnInPlaceAsset> m_TurnInPlaceAssets;

#pragma endregion
#pragma endregion
#pragma region aiming
private:
	void CalculateSmoothedRotation(float fDeltaTime, float fInterpSpeed);
	void CalculateAimingAngles();
	void CalculateSpineRotation();
	void CalculateYawTime();

private:
	FRotator m_rSmoothedRotation;
	FRotator m_rSpineRotation;

	FVector2D m_v2DAimingAngle;
	FVector2D m_v2DSmoothedAimingAngle;

	float m_fInputYawOffsetTime;
	float m_fForwardYawOffsetTime;
	float m_fLeftYawOffsetTime;
	float m_fRightYawOffsetTime;

#pragma endregion

};