// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTService_Check.h"

UBTService_Check::UBTService_Check() {

	bCreateNodeInstance = true;
}

void UBTService_Check::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	AOEntityAIController* ownerEntityController = dynamic_cast<AOEntityAIController*>(OwnerComp.GetAIOwner());

	if (ownerEntityController) {

		ATroll* player = dynamic_cast<ATroll*>(ownerEntityController->GetWorld()->GetFirstPlayerController()->GetPawn());

		if (player) {
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(ownerEntityController->targetKeyId, player);
		}
	}
}




