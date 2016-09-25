// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "Ontology/OEdification.h"
#include "PlotGenerator.h"


// Sets default values
APlotGenerator::APlotGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	// Herbivore
	static ConstructorHelpers::FObjectFinder<UBlueprint> HerbivoreBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Herbivore.BP_Herbivore'"));
	if (HerbivoreBlueprint.Object) {
		BP_Herbivore = (UClass*)HerbivoreBlueprint.Object->GeneratedClass;
	}

	// Bear
	static ConstructorHelpers::FObjectFinder<UBlueprint> BearBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Bear.BP_Bear'"));
	if (BearBlueprint.Object) {
		BP_Bear = (UClass*)BearBlueprint.Object->GeneratedClass;
	}

	// Wolf
	static ConstructorHelpers::FObjectFinder<UBlueprint> WolfBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Wolf.BP_Wolf'"));
	if (WolfBlueprint.Object) {
		BP_Wolf = (UClass*)WolfBlueprint.Object->GeneratedClass;
	}

	// Associates types of plots with identifiers
	_plotDictionary = { { TypeOfPlot::aggressive,{ _ATTACK_PLOT, _DESTROY_PLOT } },
	{ TypeOfPlot::resources,{ _BUILD_PLOT } },
	{ TypeOfPlot::possessive ,{ _ROBBERY_PLOT } },
	{ TypeOfPlot::thankful,{ _GIVE_PLOT, _HELP_PLOT } },
	{ TypeOfPlot::world,{ _WAR_PLOT } } };
}

// Called when the game starts or when spawned
void APlotGenerator::BeginPlay()
{
	Super::BeginPlay();

	Utilities::SaveStringToFile("\n\n\nNew game\n\n\n", Utilities::SavePath, Utilities::PlotFile);
	Utilities::SaveStringToFile("\n\n\nNew game\n\n\n", Utilities::SavePath, Utilities::ReportFile);

	// HARD CODED
	// FOREST
	_stampedeSpawnArea = FVector(-55, 10426, 0);

	//INSERT WORLD PLOTS FROM THE BEGINNING
	
	_worldPlots.push_back(new Stampede(ERace::R_Bear, _stampedeSpawnArea, this));
	_worldPlots.push_back(new Stampede(ERace::R_Wolf, _stampedeSpawnArea, this));
	_worldPlots.push_back(new Stampede(ERace::R_Herbivore, _stampedeSpawnArea, this));

}

// Called every frame
void APlotGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	ThreadManager::Tick();

	// PLOT SPAWN

	if (_timeToSpawnPlot < _TIME_TO_SPAWN)
		_timeToSpawnPlot += DeltaTime;
	else {
		int i = 1;
		while (_currentPlotsInAction <= _MAX_PLOTS && i <= 3) {

			if (rand() % 100 < 20+log(_pReportLog.Num()+1)*37.533f) {
			GetPlotFromReportLog();
				if (!_reactivePlots.empty()) {
					SpawnReactivePlot();
				}
			}
			else if (rand() % 100 < 15*i) {
				SpawnAmbitionPlot();
			}

			else if (rand() % 100 < (_MAX_STAMPEDES - _stampedeCount)) {
				SpawnWorldPlot();
				_stampedeCount++;
				break;
			}
			i++;
		}
		_timeToSpawnPlot = 0;
	}
}

void APlotGenerator::ChangeCurrentPlotsInAction(int dif)
{
	_currentPlotsInAction += dif;
}

bool APlotGenerator::SpawnReactivePlot()
{
	if (_reactivePlots.size() > 0) {

		BasePlot* currentPlot = nullptr;
		currentPlot = _reactivePlots.at(0);
		currentPlot->InitPlot();
		_reactivePlots.erase(_reactivePlots.begin());

		UOEntity* plotEntity = nullptr;
		plotEntity = currentPlot->GetMainEntity();
		if (plotEntity->IsValidItem()) {
			//if(!((WarPlot*)currentPlot))
			_currentPlotsInAction++;
			plotEntity->AddCurrentPlot(currentPlot);
			plotEntity->RethinkState();
			return true;
		}	
	}
	return false;
}

bool APlotGenerator::SpawnAmbitionPlot()
{
	Ambition ambition = Ambition(this, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UOEntity>());
	UOEntity* entity = nullptr;
	entity = _notoriousEntities.Num() > 0 ? _notoriousEntities[rand() % _notoriousEntities.Num()] : nullptr;

	if (entity->IsValidItem()) {
		BasePlot* ambitionPlot = ambition.GenerateAmbitionForEntity(entity);

		if (ambitionPlot) {
			_currentPlotsInAction++;
			ambitionPlot->InitPlot();
			entity->AddCurrentPlot(ambitionPlot);
			entity->RethinkState();
			return true;
		}
	}
	return false;
}

bool APlotGenerator::SpawnWorldPlot()
{
	int randPlot = rand()% _worldPlots.size();
	_worldPlots[randPlot]->InitPlot();
	_worldPlots[randPlot]->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);
	_worldPlots[randPlot]->PrintSentence(this, "", TypeOfAmbition::noAmbition);

	return true;
}

void APlotGenerator::AddReportToLog(Report* newReport)
{
	if (!_pReportLog.Contains(newReport)) {
		_pReportLog.HeapPush(newReport, Report::ReportNotoriety());
		newReport->SaveReportToFile(Utilities::SavePath, Utilities::ReportFile);
	}
}

bool APlotGenerator::ContainsReport(Report* newReport) {

	for (int i = 0; i < _pReportLog.Num(); i++){
		if (newReport->GetTag() == _pReportLog[i]->GetTag()) {
			if (newReport->GetTag() == Report::ReportTag::relation) {
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetEntity() == newReport->GetTargetEntity() && _pReportLog[i]->GetMotivationName() == newReport->GetMotivationName() && _pReportLog[i]->GetType() == newReport->GetType()) {
					return true;
				}
			}
			else if (newReport->GetTag() == Report::ReportTag::ownership){
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetOwnable() == newReport->GetTargetOwnable() && _pReportLog[i]->GetMotivationName() == newReport->GetMotivationName() && _pReportLog[i]->GetType() == newReport->GetType()) {
						return true;
				}
			}
		}
	}
	return false;
}

// Removes X reports from _pReportLog and converts them into plots
// It is verified that each report is valid before converting it
// When creating the plot, involved entities are obtained from redundant plots and these are deleted from the log
// Plot types are also validates and removed if not valid
void APlotGenerator::GetPlotFromReportLog() {

	if (_pReportLog.Num() > 0) {

		Report* currentReport = nullptr;
		_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());
		vector<string> plotCandidates = GetPlotsOfType(currentReport->GetType());

		bool plotIsValid = false;
		BasePlot* newPlot = nullptr;
		string plot = "";

		while (!plotIsValid && plotCandidates.size() > 0) {

			int randType = rand() % plotCandidates.size();
			plot = plotCandidates[randType];

			if (plot == _ATTACK_PLOT) {
				newPlot = new AttackPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivationName(), currentReport->GetMotivationRace());
				plotIsValid = ValidateAttackPlot((AttackPlot*) newPlot);

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
					currentReport->GetTargetEntity()->ChangeNotoriety(2);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}

			else if (plot == _DESTROY_PLOT) {

				UOEdification* targetEdification = nullptr;

				if (!currentReport->GetTargetEntity()->IsPlayer) {

					for (OOwnership* o : currentReport->GetTargetEntity()->GetPossessions()) {
						if (o->GetOwnable()->IsA<UOEdification>()) {
							if (!((UOEdification*)o->GetOwnable())->GetIsDestroyed()) {
								targetEdification = (UOEdification*)o->GetOwnable();
								break;
							}
						}
					}
				}
				
				if (targetEdification->IsValidItem()) {

					newPlot = new DestroyPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), targetEdification, currentReport->GetMotivationName(), currentReport->GetMotivationRace());
					plotIsValid = ValidateDestroyPlot((DestroyPlot*)newPlot);

					if (plotIsValid) {
						currentReport->GetReportEntity()->ChangeNotoriety(3);
						currentReport->GetTargetEntity()->ChangeNotoriety(2);
					}
					else plotCandidates.erase(plotCandidates.begin() + randType);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == _BUILD_PLOT) {
				newPlot = new BuildPlot(currentReport->GetReportEntity(), (UOEdification*)currentReport->GetTargetOwnable(), currentReport->GetMotivationName(), currentReport->GetMotivationRace());
				plotIsValid = ValidateBuildPlot((BuildPlot*)newPlot);

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == _HELP_PLOT) {
				newPlot = new HelpPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivationName(), currentReport->GetMotivationRace());
				plotIsValid = ValidateHelpPlot((HelpPlot*)newPlot);

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == _GIVE_PLOT) {

				UOOwnable* ownableToGive = nullptr;

				for (OOwnership* o : currentReport->GetReportEntity()->GetPossessions()) {
					if (!o->GetOwnable()->IsA<UOEdification>() && o->GetOwnable()->GetRarityAsInt() > 2 || currentReport->GetTargetEntity()->GetOwnershipWith(o->GetOwnable()) && currentReport->GetTargetEntity()->GetOwnershipWith(o->GetOwnable())->GetWorth() > 50) {
						ownableToGive = o->GetOwnable();
						break;
					}
				}
				if (ownableToGive->IsValidItem()) {
					newPlot = new GivePlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), ownableToGive, currentReport->GetMotivationName(), currentReport->GetMotivationRace());
					plotIsValid = ValidateGiftPlot((GivePlot*)newPlot);
				}

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == _ROBBERY_PLOT) {

				UOEntity* targetEntity = nullptr;

				for (UOEntity* e : allEntities) {
					if (currentReport->GetMotivationName() == e->GetItemName()) {
						targetEntity = e;
						break;
					}
				}

				if (targetEntity->IsValidItem()) {
					newPlot = new StealPlot(currentReport->GetReportEntity(), targetEntity, currentReport->GetTargetOwnable(), currentReport->GetMotivationName(), currentReport->GetMotivationRace());
					plotIsValid = ValidateStealPlot((StealPlot*)newPlot);
				}

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == _WAR_PLOT && WarCount == 0) {
				newPlot = new WarPlot(currentReport->GetReportEntity());
				plotIsValid = ValidateWarPlot((WarPlot*) newPlot);

				if (plotIsValid)  {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
					WarCount++;

					for (UOEntity* e : allEntities) {
						newPlot->AddInvolvedInPlot(e);
					}
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}

			// Unknown type
			else plotCandidates.erase(plotCandidates.begin() + randType);
		}

		if (plotIsValid) {
			if (!newPlot->GetIsExclusive()) {
				for (UOEntity* entity : WeHaveALotInCommon(currentReport)) {
					if (!entity->IsValidItem())
						continue;
					if(plot != _WAR_PLOT)
						newPlot->AddInvolvedInPlot(entity);
				}
			}
			_reactivePlots.push_back(newPlot);
		}
	}
}

vector<string> APlotGenerator::GetPlotsOfType(TypeOfPlot type)
{
	return _plotDictionary.at(type);
}

// Looks for coincidences between the given plot and the log
// Entities are returned as helpers
vector<UOEntity*> APlotGenerator::WeHaveALotInCommon(Report* report) {

	Report::ReportTag tag = report->GetTag();
	vector<UOEntity*> helpers;

	int i = 0;

	if (tag == Report::ReportTag::ownership) {

		while(i < _pReportLog.Num()){
			if (_pReportLog[i]->GetTag() == tag) {

				if (!_pReportLog[i]->GetReportEntity()->IsValidItem()) {
					_pReportLog.RemoveAt(i);
					continue;
				}
				if (_pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetOwnable() == report->GetTargetOwnable() && _pReportLog[i]->GetMotivationName() == report->GetMotivationName()) {
					if(_pReportLog[i]->GetReportEntity()->GetIntegrity()>0)
						helpers.push_back(_pReportLog[i]->GetReportEntity());
					_pReportLog.RemoveAt(i);
				}
				else i++;
			}
			else i++;
		}
	}
	else if (tag == Report::ReportTag::relation) {

		while (i < _pReportLog.Num()) {
			if (_pReportLog[i]->GetTag() == tag) {

				if (!_pReportLog[i]->GetReportEntity()->IsValidItem() || !_pReportLog[i]->GetTargetEntity()->IsValidItem()) {
					_pReportLog.RemoveAt(i);
					continue;
				}

				if (_pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetEntity() == report->GetTargetEntity() && _pReportLog[i]->GetMotivationName() == report->GetMotivationName()) {
					if (_pReportLog[i]->GetReportEntity()->GetIntegrity()>0)
						helpers.push_back(_pReportLog[i]->GetReportEntity());
					_pReportLog.RemoveAt(i);
				}
				else i++;
			}
			else i++;
		}
	}
	else if (tag == Report::ReportTag::world) {

		while (i < _pReportLog.Num()) {
			if (_pReportLog[i]->GetTag() == tag) {
				if (!_pReportLog[i]->GetReportEntity()->IsValidItem()) {
					_pReportLog.RemoveAt(i);
					continue;
				}
				else{
					helpers.push_back(_pReportLog[i]->GetReportEntity());
					_pReportLog.RemoveAt(i);
				}
			}
			else i++;
		}
	}
	return helpers;
}


// PLOT VALIDATION
bool APlotGenerator::ValidateAttackPlot(AttackPlot * plot)
{
	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetTargetEntity());
}
bool APlotGenerator::ValidateDestroyPlot(DestroyPlot * plot)
{
	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetTargetEntity()) && !plot->GetTargetEdification()->GetIsDestroyed();
}
bool APlotGenerator::ValidateBuildPlot(BuildPlot* plot)
{
	return plot && ValidateEntity(plot->GetMainEntity()) && plot->GetTargetEdification()->GetIsDestroyed();
}
bool APlotGenerator::ValidateHelpPlot(HelpPlot* plot) {
	
	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetTargetEntity());
}
bool APlotGenerator::ValidateGiftPlot(GivePlot* plot) {

	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetTargetEntity());
}
bool APlotGenerator::ValidateStealPlot(StealPlot* plot) {
	
	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetTargetEntity());
}
bool APlotGenerator::ValidateGetPlot(GetPlot* plot) {

	return plot && ValidateEntity(plot->GetMainEntity()) && !plot->GetTargetOwnable()->GetIsGrabbed();
}
bool APlotGenerator::ValidateWarPlot(WarPlot* plot) {

	return plot && ValidateEntity(plot->GetMainEntity()) && ValidateEntity(plot->GetMainEntity()->GetMostHated());
}
bool APlotGenerator::ValidateDefendPlot(DefendPlot* plot) {
	return plot && ValidateEntity(plot->GetMainEntity()) &&
		(plot->GetTarget()->IsA<UOEntity>() ?
			ValidateEntity((UOEntity*)(plot->GetTarget())) :
			!((UOEdification*)plot->GetTarget())->GetIsDestroyed());
}

vector<UOOwnable*> APlotGenerator::GetValuables()
{
	return _valuables;
}
void APlotGenerator::AddValuable(UOOwnable * valuable)
{
	_valuables.push_back(valuable);
}


// NOTORIETY METHODS
vector<UOEntity*> APlotGenerator::GetNotoriousEntitiesByRace(ERace race)
{
	vector<UOEntity*> entities;

	for (UOEntity* e : _notoriousEntities) {
		if (!e->IsValidItem())
			continue;

		if (e->GetRace() == race)
			entities.push_back(e);
	}

	return entities;
}
vector<UOEntity*> APlotGenerator::GetNotoriousEntities()
{
	vector<UOEntity*> entities;

	for (UOEntity* e : _notoriousEntities) {
		if (!e->IsValidItem())
			continue;

		entities.push_back(e);
	}
			

	return entities;
}
void APlotGenerator::AddNotorious(UOEntity * notorious)
{
	if (_notoriousEntities.Num() < _MAX_NOTORIOUS) {

		if (!_notoriousEntities.Contains(notorious)) {
			_notoriousEntities.HeapPush(notorious, UOEntity::EntityNotoriety());
		}
		else {
			_notoriousEntities.Remove(notorious);
			_notoriousEntities.HeapPush(notorious, UOEntity::EntityNotoriety());
		}
	}
	else if (notorious->GetNotoriety() > _notoriousEntities[1]->GetNotoriety()) {

		if (!_notoriousEntities.Contains(notorious)) {
			_notoriousEntities.HeapRemoveAt(1, UOEntity::EntityNotoriety());
			_notoriousEntities.HeapPush(notorious, UOEntity::EntityNotoriety());
		}
		else {
			_notoriousEntities.Remove(notorious);
			_notoriousEntities.HeapPush(notorious, UOEntity::EntityNotoriety());
		}
	}
}
void APlotGenerator::DeleteNotorious(UOEntity * entity)
{
	if(entity && _notoriousEntities.Contains(entity))
		_notoriousEntities.Remove(entity);
}


// PLOT GUI
void APlotGenerator::SetCurrentPlotString(FString sentence)
{
	_currentPlotString = sentence;
}
void APlotGenerator::SetCurrentPlotTypeColor(FLinearColor color)
{
	_currentPlotTypeColor = color;
}


// WORLD EVENTS
float APlotGenerator::GetOverallHateAgainstRace(ERace race)
{
	vector<UOEntity*> poblationSample;

	float sum = 0;

	if (poblationSample.size() > 0) {
		if (race == ERace::R_Human)
			poblationSample = GetNotoriousEntitiesByRace(ERace::R_Goblin);
		else poblationSample = GetNotoriousEntitiesByRace(ERace::R_Human);

		if (poblationSample.size() == 0) return 100;

		for (UOEntity* e : poblationSample) {
			if (!e->IsValidItem())
				continue;

			sum += e->GetAppreciationToOtherRace(race == ERace::R_Human? ERace::R_Goblin : ERace::R_Human);
		}
		return sum / poblationSample.size();
	}

	return sum;
}


// UTILITIES
FVector APlotGenerator::RandomDisplacement(int radius) {

	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}
vector<UOEntity*> APlotGenerator::SpawnEntities(int num, ERace race, FVector spawnLocation) {

	FActorSpawnParameters SpawnParams;
	vector<UOEntity*> spawnedHeard;
	ACharacter* creatureToSpawn;

	spawnLocation *= FVector(1, 1, 0);

	for (int i = 0; i < num; i++) {

		switch (race) {
			case ERace::R_Bear:
				creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Bear, spawnLocation + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
				break;
			case ERace::R_Wolf:
				creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Wolf, spawnLocation + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
				break;
			case ERace::R_Herbivore:
				creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Herbivore, spawnLocation + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
				break;
			default:
				creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Bear, spawnLocation + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
				break;
		}

		if (creatureToSpawn) {
			float scale = rand() % 10 + 6;
			creatureToSpawn->SetActorScale3D(FVector(scale / 10, scale / 10, scale / 10));
			creatureToSpawn->SetActorLocation(creatureToSpawn->GetActorLocation() * FVector(1, 1, 0) + FVector(0, 0, 100));

			UOEntity* spawnedEntity = nullptr;
			spawnedEntity = creatureToSpawn->FindComponentByClass<UOEntity>();

			spawnedEntity->SetRace(ERace::R_Bear);
			spawnedEntity->SetJob(EJob::J_Predator);
			spawnedEntity->SetHome(nullptr);
			spawnedEntity->SetPlotGenerator();
			spawnedEntity->GenerateTraits();

			for (UOEntity* heardEntity : spawnedHeard) {
				ORelation* r = spawnedEntity->AddRelationship(heardEntity);
				r->ChangeAppreciation(20);
				r = heardEntity->AddRelationship(spawnedEntity);
				r->ChangeAppreciation(20);
			}

			spawnedHeard.push_back(spawnedEntity);
		}
	}
	return spawnedHeard;
}

bool APlotGenerator::ValidateEntity(UOEntity* e) {
	if (e)
		return e->IsValidItem();
	return false;
}