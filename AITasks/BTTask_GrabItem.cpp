// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GrabItem.h"


EBTNodeResult::Type UBTTask_GrabItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	
	AEntityAIController* entityController = dynamic_cast<AEntityAIController*>(OwnerComp.GetAIOwner());
	UOEntity* entity = entityController->GetPawn()->FindComponentByClass <UOEntity>();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	
	/*UItem* item = (UItem*) blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"));
	if(item)
		entity->GrabItem(item);
	else {
		item = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Ownable"));
		if (item)
			entity->GrabItem(item);
	}*/

	UItem* item = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Item"));
	if(!item)
		item = (UItem*)blackboard->GetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID("Ownable"));

	/*bool inInventory = false;
	if (item) {
		for (UOOwnable* o : entity->GetInventory()) {
			if ((UOOwnable*)item == o) {
				inInventory = true;
				break;
			}
		}

		if (inInventory) {
			entity->GrabFromInventory((UOOwnable*)item);
		}
		else {
			entity->GrabItem(item);
		}
	}*/
	if(item->IsA<UOOwnable>() && !((UOOwnable*)item)->GetIsGrabbed())
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