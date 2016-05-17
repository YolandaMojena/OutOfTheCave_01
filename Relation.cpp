// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Entity.h"
#include "Thing.h"
#include "Relation.h"

Relation::Relation(AEntity* entity, KindOfRelation kind) : otherEntity(entity), relation(kind) {}

Relation::Relation(AThing* thing, KindOfRelation kind) : otherThing(thing), relation(kind) {}

Relation::Relation(KindOfRelation kind) : relationWithPlayer(kind) {}

Relation::~Relation() {}
