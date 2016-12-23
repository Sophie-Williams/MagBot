#include "WorkerManager.h"

using namespace MagBot;
using namespace BWAPI;
using namespace Filter;

WorkerManager::WorkerManager()
{
	
}


WorkerManager::~WorkerManager()
{
}

WorkerManager & WorkerManager::Instance()
{
	static WorkerManager instance;
	return instance;
}

void WorkerManager::update()
{
	updateWorkerTask();

	// TODO Move for loop in their respective handle function, to make the conditionals more specific
	for (auto & unit : _worker.getWorkers())
	{
		// TODO check these conditionals, if can move to init constructor of worker instead and some can keep here for the update
		if (!unit->exists())
			continue;

		// Ignore the unit if it has one of the following status ailments
		if (unit->isLockedDown() || unit->isMaelstrommed() || unit->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (unit->isLoaded() || !unit->isPowered() || unit->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!unit->isCompleted())// || unit->isConstructing())
			continue;

		showDebugWorkerInfo(unit);

		handleMineralWorkers(unit);

		handleIdleWorkers(unit);
	}
}

void WorkerManager::updateWorkerTask()
{
	_worker.update();
}

BWAPI::Unit WorkerManager::getBuilder()
{
	BWAPI::Unit closestMiningWorker = nullptr;

	for (auto & unit : _worker.getWorkers())
	{
		// TODO check closest
		//closestMiningWorker = unit;
		//Position pos = unit->getPosition();
		//Broodwar->drawBoxScreen(pos, pos, Colors::Orange, false);// (pos, 10, Colors::Orange, false);
		return unit;
	}
	return closestMiningWorker;
}

// TODO buildWithSpacing(int)
// AND make sure there's is space between buildings (one tile)?

// drawBox the second it is under construction (permanent) instead of updating every frame

// TODO could add int or a type Priority for the BuildOrderManager
void WorkerManager::build(BWAPI::UnitType unitType)
{
	BWAPI::Unit builder = getBuilder();
	

	// TODO modify to pylons built, that exist then if !canBuildHere then continue, if can build, if cant find then build pylon,
	
	for (auto & unit : BWAPI::Broodwar->self()->getUnits())
	{
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
		{
			TilePosition pos = unit->getTilePosition();
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(unitType, pos);

			if (Broodwar->canBuildHere(targetBuildLocation, unitType, builder))
			{
				_worker.setWorkerTask(builder, Worker::WorkerTask::Build, unitType);
				Broodwar->drawBoxMap(Position(targetBuildLocation), Position(targetBuildLocation + unitType.tileSize()), Colors::Green);
				builder->build(unitType, targetBuildLocation);
				//Broodwar->drawBoxMap(Position(targetBuildLocation), Position(targetBuildLocation), Colors::Blue);
				//Broodwar->drawCircleScreen(pos, 10, Colors::Blue, false);
				//Broodwar->drawTextScreen(0, 0, "Build GATEWAY: OK");
			}		
			// TODO check return here or inside the condition
			return;
		}
	}

}

void WorkerManager::showDebugWorkerInfo(BWAPI::Unit unit)
{
	Position pos = unit->getPosition();

	if (_worker.getWorkerTask(unit) == Worker::WorkerTask::Mine)
	{		
		BWAPI::Broodwar->drawTextMap(pos, "MINER");
	}
	else if (_worker.getWorkerTask(unit) == Worker::WorkerTask::Build)
	{
		BWAPI::Broodwar->drawTextMap(pos, "BUILDER");
	}
}

void WorkerManager::handleMineralWorkers(BWAPI::Unit unit)
{
	if (unit->isIdle() && _worker.getWorkerTask(unit) == Worker::WorkerTask::Mine) // TO UNCOMMENT:  // 
	{
		// Order workers carrying a resource to return them to the center,
		// otherwise find a mineral patch to harvest.
		if (unit->isCarryingGas() || unit->isCarryingMinerals())
		{
			unit->returnCargo();
		}
		else if (!unit->getPowerUp())  // The worker cannot harvest anything if it
		{                             // is carrying a powerup such as a flag
			// Harvest from the nearest mineral patch or gas refinery
			if (!unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery)))
			{
				// If the call fails, then print the last error message
				Broodwar << Broodwar->getLastError() << std::endl;
			}
		}
	}
}

void WorkerManager::handleIdleWorkers(BWAPI::Unit unit)
{
	for (auto & unit : _worker.getWorkers())
	{
		if (_worker.getWorkerTask(unit) == Worker::WorkerTask::Build && unit->isIdle())
		{
			_worker.setWorkerTask(unit, Worker::WorkerTask::Mine);
		}
	}
}


/*void WorkerManager::addWorker()
{
	
}*/

// other managers that need workers call this when they're done with a unit
/*void WorkerManager::finishedWithWorker(BWAPI::Unit unit)
{
	//UAB_ASSERT(unit != nullptr, "Unit was null");
	if (!unit)
		return;
	//BWAPI::Broodwar->printf("BuildingManager finished with worker %d", unit->getID());
	if (_worker.getWorkerTask(unit) != Worker::Scout)
	{
		_worker.setWorkerTask(unit, Worker::Idle);
	}
}*/

