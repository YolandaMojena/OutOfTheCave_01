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

	//Insert on of each
	worldPlots.push_back(new Stampede(ERace::R_Bear, GetActorLocation(), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UOEntity>(), rand() % 10 + 5, this));
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
			//if (rand() % 100 >98) {
				//SpawnWorldPlot();
				//_timeToSpawnPlot = 0;
			//}
			//else {
				if(reactivePlots.empty())
					GetPlotFromReportLog();
				if (reactivePlots.size() > 0) {
					SpawnReactivePlot();
					_timeToSpawnPlot = 0;
				}
			//}
		}
	}
}

bool APlotGenerator::ValidateReport(Report* report)
{
		//return (report->GetReportEntity() == nullptr);
	return true;
}

void APlotGenerator::ChangeCurrentPlotsInAction(int dif)
{
	_currentPlotsInAction += dif;
}

void APlotGenerator::SpawnReactivePlot()
{
	if (reactivePlots.size() > 0) {
		BasePlot* currentPlot = reactivePlots.at(0);
		reactivePlots.erase(reactivePlots.begin());
		currentPlot->PrintSentence();
		currentPlot->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);

		UOEntity* plotEntity = currentPlot->GetMainEntity();
		plotEntity->AddCurrentPlot(currentPlot);
		plotEntity->SetMainPlotEntity(plotEntity);
		if (plotEntity->GetCurrentState() == UOEntity::State::idle) {
			plotEntity->SetState(UOEntity::State::plot);
			_currentPlotsInAction++;
		}
	}
}

void APlotGenerator::SpawnWorldPlot()
{
	int randPlot = rand()% worldPlots.size();
	worldPlots[randPlot]->InitPlot();
	worldPlots[randPlot]->SavePlotToFile(Utilities::SavePath, Utilities::PlotFile);
}

void APlotGenerator::AddReportToLog(Report* newReport)
{
	if (!ContainsReport(newReport)) {
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

		bool validReport = ValidateReport(currentReport);

		while (!validReport && _pReportLog.Num() > 0) {
			_pReportLog.HeapPop(currentReport, Report::ReportNotoriety());
			validReport = ValidateReport(currentReport);
		}

		if (validReport) {

			vector<string> plotCandidates = plotDictionary.GetPlotsOfType(currentReport->GetType());

			bool plotIsValid = false;
			BasePlot* newPlot = nullptr;

			while (!plotIsValid) {

				int randType = rand() % plotCandidates.size();
				string plot = plotCandidates[randType];

				if (plot == strings.ATTACK_PLOT) {
					newPlot = new AttackPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivation());
					plotIsValid = ValidateAttackPlot((AttackPlot*) newPlot);

					if (plotIsValid) {
						newPlot->InitPlot();
						currentReport->GetReportEntity()->ChangeNotoriety(3);
						currentReport->GetTargetEntity()->ChangeNotoriety(2);
					}
					else plotCandidates.erase(plotCandidates.begin() + randType);
				}

				else if (plot == strings.DESTROY_PLOT) {
					newPlot = new DestroyPlot(currentReport->GetReportEntity(), currentReport->GetTargetEntity(), currentReport->GetMotivation());
					plotIsValid = ValidateDestroyPlot((DestroyPlot*)newPlot);	

					if (plotIsValid) {
						newPlot->InitPlot();
						currentReport->GetReportEntity()->ChangeNotoriety(3);
						currentReport->GetTargetEntity()->ChangeNotoriety(2);
					}
					else plotCandidates.erase(plotCandidates.begin() + randType);
				}
				else if (plot == strings.BUILD_PLOT) {
					newPlot = new BuildPlot(currentReport->GetReportEntity(), (UOEdification*)currentReport->GetTargetOwnable(), currentReport->GetMotivation());
					plotIsValid = true;

					if (plotIsValid) {
						newPlot->InitPlot();
						currentReport->GetReportEntity()->ChangeNotoriety(3);
					}
					else plotCandidates.erase(plotCandidates.begin() + randType);
				}

				// Unknown type
				else plotCandidates.erase(plotCandidates.begin() + randType);

				if (plotCandidates.size() == 0)
					break;
			}


			if (plotIsValid) {
				if (!newPlot->GetIsExclusive()) {
					for (UOEntity* entity : WeHaveALotInCommon(currentReport)) {
						newPlot->AddInvolvedInPlot(entity);
						entity->ChangeNotoriety(1);
					}
				}
				newPlot->BuildSentence();
				reactivePlots.push_back(newPlot);
			}
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

	while (i < _pReportLog.Num()) {

		if (_pReportLog[i]->GetTag() == tag && tag == Report::ReportTag::ownership) {
			if ( _pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetOwnable() == report->GetTargetOwnable()) {
					helpers.push_back(_pReportLog[i]->GetReportEntity());
				_pReportLog.RemoveAt(i);
			}
			else i++;
		}
		else if (_pReportLog[i]->GetTag() == tag && tag == Report::ReportTag::relation) {
			if ( _pReportLog[i]->GetType() == report->GetType() && _pReportLog[i]->GetTargetEntity() == report->GetTargetEntity()) {
				helpers.push_back(_pReportLog[i]->GetReportEntity());
				_pReportLog.RemoveAt(i);
			}
			else i++;
		}
	}

	return helpers;
}

bool APlotGenerator::ValidateAttackPlot(AttackPlot * plot)
{
	return true;
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


vector<UOEntity*> APlotGenerator::SpawnEntities(int num, ERace race) {

	FActorSpawnParameters SpawnParams;
	vector<UOEntity*> spawnedHeard;

	switch (race) {
		case ERace::R_Bear: {

			for (int i = 0; i < num; i++) {
				ACharacter* creatureToSpawn = GetWorld()->SpawnActor<ACharacter>(BP_Bear, GetActorLocation() + RandomDisplacementVector(1000), GetActorRotation(), SpawnParams);
				
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

vector<UOEntity*> APlotGenerator::GetNotoriousEntitiesByRace(ERace race)
{
	vector<UOEntity*> entities;

	for (UOEntity* e : _notoriousEntities)
		if (e->GetRace() == race)
			entities.push_back(e);

	return entities;
}

FVector APlotGenerator::RandomDisplacementVector(int radius)
{
	return FVector(rand() % (2 * radius) - radius, rand() % (2 * radius) - radius, 0);
}




