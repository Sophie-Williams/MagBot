#pragma once

#include "Common.h"

namespace MagBot
{

	class BuildingManager
	{

		// TODO container for all protoss buildings include or exclude pylons?
		// TODO update on each frame the count of minerals and gas owned -> make sure when something is in queue, other orders aren't
		// interfering

		BWAPI::Unitset _buildings;
		std::vector<BWAPI::Unit> _buildingsUnderConstruction;

		// TODO find better way for this since i want to be able to select one BWAPI::Unit instance
		std::map<BWAPI::UnitType, int> _buildingsOwnedMap;
		std::map<BWAPI::UnitType, int> _buildingsDestroyedMap;
		int _buildingDestroyed = 0;

		// TODO Building instance
		// building stats: hp, shield, armor, mineral cost, gas cost, position, tileposition, damage taken, etc.

	public:
		BuildingManager();
		~BuildingManager();

		void update();

		void removeBuildingsDestroyed();
		void addBuildingsUnderConstruction();
		void removeBuildingsCompleted();

		void showDebugBuildings();
		void showBuildTimeBuildings();
		void showOwnedOrDestroyedBuildings();

		const BWAPI::Unitset getBuildings() const { return _buildings; }
		
		static BuildingManager & Instance();
	};

}