// Fill out your copyright notice in the Description page of Project Settings.

#include "OutOfTheCave_01.h"
#include "PlotGenerator.h"


// Sets default values
APlotGenerator::APlotGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	static ConstructorHelpers::FObjectFinder<UBlueprint> BearBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Bear.BP_Bear'"));
	if (BearBlueprint.Object) {
		BP_Bear = (UClass*)BearBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void APlotGenerator::BeginPlay()
{
	Super::BeginPlay();

	//INSERT WORLD PLOTS FROM THE BEGINNING
	_worldPlots.push_back(new Stampede(ERace::R_Bear, GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UOEntity>(), rand() % 10 + 5, this));
}

// Called every frame
void APlotGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// PLOT SPAWN

	if (_timeToSpawnPlot < _TIME_TO_SPAWN)
		_timeToSpawnPlot += DeltaTime;

	else {
		if (_currentPlotsInAction <= _MAX_PLOTS) {

			if (rand() % 100 <= 25 + (25 * _MAX_PLOTS - _currentPlotsInAction) ) {

				if (_reactivePlots.empty()) {
					GetPlotFromReportLog();
				}
				if (_reactivePlots.size() > 0) {
					//_reactivePlots[0]->PrintSentence();
					SpawnReactivePlot();
					_timeToSpawnPlot = 0;
				}
			}
			else if (rand() % 100 <= (25 /_currentPlotsInAction + 1)) {

				SpawnAmbitionPlot();
				_timeToSpawnPlot = 0;
			}
			else if (rand() % 100 <= 5) {
				//SpawnWorldPlot();
				_timeToSpawnPlot = 0;
			}
		}
	}
}

void APlotGenerator::ChangeCurrentPlotsInAction(int dif)
{
	_currentPlotsInAction += dif;
}

bool APlotGenerator::SpawnReactivePlot()
{
	if (_reactivePlots.size() > 0) {

		BasePlot* currentPlot = _reactivePlots.at(0);
		_reactivePlots.erase(_reactivePlots.begin());

		UOEntity* plotEntity = currentPlot->GetMainEntity();
		plotEntity->AddCurrentPlot(currentPlot);
			//plotEntity->SetState(UOEntity::State::plot);
		plotEntity->RethinkState();

		return true;
	}
	return false;
}

bool APlotGenerator::SpawnAmbitionPlot()
{
	Ambition ambition = Ambition(this, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UOEntity>());
	UOEntity* entity = _notoriousEntities.Num() > 0 ? _notoriousEntities[rand() % _notoriousEntities.Num()] : nullptr;

	if (entity) {

		BasePlot* ambitionPlot = ambition.GenerateAmbitionForEntity(entity);

		if (ambitionPlot) {
			ambitionPlot->InitPlot();
			entity->AddCurrentPlot(ambitionPlot);
				//entity->SetState(UOEntity::State::plot);
			entity->RethinkState();
			return true;
		}
		else return false;
	}
	else return false;
}

bool APlotGenerator::SpawnWorldPlot()
{
	int randPlot = rand()% _worldPlots.size();
	_worldPlots[randPlot]->InitPlot();
	_worldPlots[randPlot]->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);
	_worldPlots[randPlot]->PrintSentence();

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
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetEntity() == newReport->GetTargetEntity() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetType() == newReport->GetType()) {
					return true;
				}
			}
			else if (newReport->GetTag() == Report::ReportTag::ownership){
				if (_pReportLog[i]->GetReportEntity() == newReport->GetReportEntity() && _pReportLog[i]->GetTargetOwnable() == newReport->GetTargetOwnable() && _pReportLog[i]->GetMotivation() == newReport->GetMotivation() && _pReportLog[i]->GetType() == newReport->GetType()) {
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

		Report* currentReport;
		_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());
		vector<string> plotCandidates = plotDictionary.GetPlotsOfType(currentReport->GetType());

		bool plotIsValid = false;
		BasePlot* newPlot = nullptr;

		while (!plotIsValid && plotCandidates.size() > 0) {

			int randType = rand() % plotCandidates.size();
			string plot = plotCandidates[randType];

			if (plot == strings.ATTACK_PLOT) {
				newPlot = new AttackPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivation());
				plotIsValid = ValidateAttackPlot((AttackPlot*) newPlot);

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
					currentReport->GetTargetEntity()->ChangeNotoriety(2);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}

			else if (plot == strings.DESTROY_PLOT) {
				newPlot = new DestroyPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivation());
				plotIsValid = ValidateDestroyPlot((DestroyPlot*)newPlot);	

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
					currentReport->GetTargetEntity()->ChangeNotoriety(2);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			else if (plot == strings.BUILD_PLOT) {
				newPlot = new BuildPlot(currentReport->GetReportEntity(), (UOEdification*)currentReport->GetTargetOwnable(), currentReport->GetMotivation());
				plotIsValid = ValidateBuildPlot((BuildPlot*)newPlot);

				if (plotIsValid) {
					currentReport->GetReportEntity()->ChangeNotoriety(3);
				}
				else plotCandidates.erase(plotCandidates.begin() + randType);
			}
			// Unknown type
			else plotCandidates.erase(plotCandidates.begin() + randType);
		}

		if (plotIsValid) {
			if (!newPlot->GetIsExclusive()) {
				for (UOEntity* entity : WeHaveALotInCommon(currentReport)) {
					newPlot->AddInvolvedInPlot(entity);
				}
			}
			newPlot->InitPlot();
			_reactivePlots.push_back(newPlot);
		}
	}
}

// Associates types of plots with plot identificators
PlotDictionary::PlotDictionary() {

	_plotDictionary = { {TypeOfPlot::aggressive, {strings.ATTACK_PLOT, strings.DESTROY_PLOT}},
	{ TypeOfPlot::resources, { strings.BUILD_PLOT } },
	{ TypeOfPlot::possessive , { strings.ROBBERY_PLOT } } };
}

vector<string> PlotDictionary::GetPlotsOfType(TypeOfPlot type)
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
				if (_pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetOwnable() == report->GetTargetOwnable()) {
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
				if (_pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetEntity() == report->GetTargetEntity()) {
					if (_pReportLog[i]->GetReportEntity()->GetIntegrity()>0)
						helpers.push_back(_pReportLog[i]->GetReportEntity());
					_pReportLog.RemoveAt(i);
				}
				else i++;
			}
			else i++;
		}
	}
	return helpers;
}

bool APlotGenerator::ValidateAttackPlot(AttackPlot * plot)
{
	return(plot->GetTargetEntity()->GetIntegrity() > 0);
}

bool APlotGenerator::ValidateDestroyPlot(DestroyPlot * plot)
{
	bool ownsEdification = false;

	for (OOwnership* o : plot->GetTargetEntity()->GetPossessions()) {

		if (o->GetOwnable()->IsA<UOEdification>()) {
			ownsEdification = true;
			break;
		}
	}
	return ownsEdification;
}

bool APlotGenerator::ValidateBuildPlot(BuildPlot* build)
{
	return build->GetTargetEdification()->GetIsDestroyed();
}


vector<UOEntity*> APlotGenerator::SpawnEntities(int num, ERace race) {

	FActorSpawnParameters SpawnParams;
	vector<UOEntity*> spawnedHeard;

	switch (race) {
		case ERace::R_Bear: {

			for (int i = 0; i < num; i++) {
				ACharacter* creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Bear, GetActorLocation() + RandomDisplacement(1000), GetActorRotation(), SpawnParams);
				
				if (creatureToSpawn) {
					float scale = rand() % 10 + 6;
					creatureToSpawn->SetActorScale3D(FVector(scale / 10, scale / 10, scale / 10));
					spawnedHeard.push_back(creatureToSpawn->FindComponentByClass<UOEntity>());
				}
			}
		}
	}

	return spawnedHeard;
}

vector<UOOwnable*> APlotGenerator::GetValuables()
{
	return _valuables;
}

void APlotGenerator::AddValuable(UOOwnable * valuable)
{
	_valuables.push_back(valuable);
}

vector<UOEntity*> APlotGenerator::GetNotoriousEntitiesByRace(ERace race)
{
	vector<UOEntity*> entities;

	for (UOEntity* e : _notoriousEntities)
		if (e->GetRace() == race)
			entities.push_back(e);

	return entities;
}

vector<UOEntity*> APlotGenerator::GetNotoriousEntities()
{
	vector<UOEntity*> entities;

	for (UOEntity* e : _notoriousEntities)
			entities.push_back(e);

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
	_notoriousEntities.Remove(entity);
}

FVector APlotGenerator::RandomDisplacement(int radius){
	
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}


/*float APlotGenerator::GetDaytime() {
	/*float p = sun->GetActorRotation().Pitch;
	float y = sun->GetActorRotation().Yaw;
	float r = sun->GetActorRotation().Roll;

	FRotator rot = sun->GetActorRotation();
	float angle = rot.Euler().Y;

	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, "\nRot: " + rot.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, "\nAbs: " + FString::SanitizeFloat(abs(p)*12.f / 90.f));

	if (p < 0) {
		if (r < -100 || r > 100) {
			return 8.f + abs(p) * 6.f / 90.f;
		}
		else {
			return 8.f + 12.f - abs(p) * 6.f / 90.f;
		}
	}
	else {
		if (r > -100 && r < 100) {
			if (abs(p) * 6.f / 90.f < 24 - 8)
				return 9.f + 12.f + abs(p) * 6.f / 90.f;
			else
				return  24.f + abs(p) * 6.f / 90.f;
		}
		else {
			return 24.f - abs(p) * 6.f / 90.f;
		}
	}


	if (r < -100 || r > 100)
		if(y < -100 || y > 100)
			return 3.f + abs(p)*12.f / 90.f;
		else
			return abs(p)*12.f / 90.f - 3.f + 12.f;
	else
		if(y < -100 || y > 100)
			return 3.f - 24.f + abs(p)*12.f / 90.f;
		else
			return 3.f + 24.f - abs(p)*12.f / 90.f;
}*/