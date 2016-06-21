// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "BTTask_GrabItem.h"

BTTask_GrabItem::BTTask_GrabItem()
{
}

BTTask_GrabItem::~BTTask_GrabItem()
{
}

void BTTask_GrabItem::GrabItem(UItem* item) {
	//	UOEntity* entity = blackboard->GetEntity()
	//	
	//	if(entity->GetStrength()/2 >= item->GetWeight()) { // should this block be managed on UOEntity::GrabItem()?
	//		if(entity->HasGrabbedItem())
	//			entity->StoreGrabbedItem();
	//		entity->GrabItem(item);
	//	}
}