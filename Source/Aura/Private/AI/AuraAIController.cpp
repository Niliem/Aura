// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAuraAIController::AAuraAIController()
{
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
    check(Blackboard);
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
    check(BehaviorTreeComponent);
}
