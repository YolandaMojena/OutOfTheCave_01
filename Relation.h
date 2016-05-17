// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AEntity;
class AThing;

class OUTOFTHECAVE_01_API Relation {

public:

	enum KindOfRelation {
		colleague, neutral, foe, owns, wants
	};

	Relation(AEntity* entity, KindOfRelation kind);
	Relation(AThing* thing, KindOfRelation kind);
	Relation(KindOfRelation kind);
	~Relation();

	AEntity* otherEntity;
	AThing* otherThing;
	KindOfRelation relationWithPlayer;
	KindOfRelation relation;
};
