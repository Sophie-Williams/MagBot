#pragma once

#include "Common.h"
#include "Macro/BuildOrderQueue.h"
#include "Macro/BuildOrder.h"
#include "Macro/WorkerManager.h"
#include <BWTA.h>
#include <string.h>

namespace MagBot
{
	using MetaPair = std::pair<MetaType, size_t>;
	using MetaPairVector = std::vector<MetaPair>;

	struct Strategy
	{
		std::string _name;
		BuildOrder _build_order;
		
		Strategy()
			: _name("None")
		{
		}

		Strategy(const std::string & name, const BuildOrder & build_order)
			: _name(name)
			, _build_order(build_order)
		{
		}
	};

	class StrategyManager
	{
		std::map<std::string, Strategy> _strategies;

		

	public:
		StrategyManager();
		~StrategyManager();

		void addStrategy(const std::string & name, Strategy & strategy);
		
		// TODO when workers destroyed, calculate enemy visible force, rebuild workers greedly or not
		// up in production? or up in army size?
		BuildOrder timedExpansion();
		BuildOrder getOpeningBuildOrder();

		BuildOrder optimizeMiningTest();
		static StrategyManager & Instance();
		
	};

}