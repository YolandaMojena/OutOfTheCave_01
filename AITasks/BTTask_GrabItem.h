// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ontology/Item.h"
#include "Ontology/OEntity.h"

/**
 * 
 */
class OUTOFTHECAVE_01_API BTTask_GrabItem
{
public:
	BTTask_GrabItem();
	~BTTask_GrabItem();
	
	void GrabItem(UItem* item);
};
