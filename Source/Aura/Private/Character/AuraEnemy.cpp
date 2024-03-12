// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterGameplayInfo.h"
#include "UI/WidgetController/EnemyWidgetController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "Aura/Aura.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAuraEnemy::AAuraEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

    HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
    HealthWidget->SetupAttachment(GetRootComponent());
    HealthWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!HasAuthority())
        return;

    AuraAIController = Cast<AAuraAIController>(NewController);
    AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
    AuraAIController->RunBehaviorTree(BehaviorTree);

    AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bIsHitReacting"), false);
    if (CharacterGameplayInfo)
        AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bIsRangedAttacker"), CharacterGameplayInfo->CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();

    InitAbilityActorInfo();
    InitializeDefaultAttributes();
    InitializeDefaultAbilities();

    GetAbilitySystemComponent()->RegisterGameplayTagEvent(AuraGameplayTags::Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
}

void AAuraEnemy::InitAbilityActorInfo()
{
    GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
    Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent())->AbilityActorInfoSet();

    EnemyWidgetController = NewObject<UEnemyWidgetController>(this, EnemyWidgetControllerClass);
    EnemyWidgetController->SetWidgetControllerParams({nullptr, nullptr, GetAbilitySystemComponent(), GetAttributeSet()});
    EnemyWidgetController->BindCallbacksToDependencies();

    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);
    HealthWidget->SetWidget(Widget);

    if (auto AuraUserWidget = Cast<UAuraUserWidget>(Widget))
    {
        AuraUserWidget->SetWidgetController(EnemyWidgetController);
    }

    EnemyWidgetController->BroadcastInitialValues();
}

void AAuraEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);
    Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* TargetActor)
{
    CombatTarget = TargetActor;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
    return CombatTarget;
}

int32 AAuraEnemy::GetCharacterLevel() const
{
    return Level;
}

void AAuraEnemy::Die_Implementation()
{
    SetLifeSpan(5.0f);
    HealthWidget->SetVisibility(false);
    AuraAIController->GetBrainComponent()->StopLogic("Dead");
    Super::Die_Implementation();
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    bHitReacting = NewCount > 0;
    GetCharacterMovement()->StopActiveMovement();

    if (AuraAIController)
        AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bIsHitReacting"), bHitReacting);
}
