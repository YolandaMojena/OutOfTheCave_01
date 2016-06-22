// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GrabItem.h"

UBTTask_GrabItem::UBTTask_GrabItem()
{
}

UBTTask_GrabItem::~UBTTask_GrabItem()
{
}

EBTNodeResult::Type UBTTask_GrabItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	UItem* item = (UItem*) blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"));

	entity->GrabItem(item);

	return EBTNodeResult::Succeeded;
	
	// = blackboard//blackboard->GetEntity()
	//	
	//	if(entity->GetStrength()/2 >= item->GetWeight()) { // should this block be managed on UOEntity::GrabItem()?
	//		if(entity->HasGrabbedItem())
	//			entity->StoreGrabbedItem();
	//		entity->GrabItem(item);
	//	}
}