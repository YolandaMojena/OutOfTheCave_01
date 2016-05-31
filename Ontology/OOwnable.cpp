// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "OEntity.h"
#include "OOwnable.h"

void UOOwnable::ReceiveDamage(float damage, UOEntity* damager) {

	if (!_destroyed) {

		_integrity -= damage;
		_attacker = damager;

		if (_integrity < MIN_INTEGRITY) {
			_destroyed = true;
			IHaveBeenDestroyedBySomeone(damager);
		}
	}
}

void UOOwnable::IHaveBeenDestroyedBySomeone(UOEntity* damager)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("I have been destroyed"));
}
