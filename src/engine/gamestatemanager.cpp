/***************************************************************************
 *   Copyright (C) 2005-2006 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/
#include "gamestatemanager.h"
#include "gamestate.h"
#include "exception.h"
#include "debugutils.h"

#include <algorithm>
#include <functional>

namespace FIFE {

	GameStateManager::GameStateManager() : m_gamestates(), m_gamestates_active() {}

	GameStateManager::~GameStateManager() {}

	GameStateManager::GameStateManager(const GameStateManager&) : m_gamestates(), m_gamestates_active() {}

	GameStateManager& GameStateManager::operator=(const GameStateManager&) {
		return *this;
	}

	void GameStateManager::registerState(GameState* gs) {
		const std::string& name = gs->getName();
		if (gameStateExists(name)) {
			PANIC_PRINT("name already used: " << name);
			return;
		}

		m_gamestates.insert(std::make_pair(name, gs));
		Log(DBGTYPE) << "registered gamestate: " << gs->getName();
	}

	void GameStateManager::unregisterState(GameState* gs) {
		type_gamestates::iterator i = m_gamestates.find(gs->getName());
		if (i == m_gamestates.end()) {
			PANIC_PRINT("tried to unregister unknown gamestate: " << gs->getName());
			return;
		}

		// TODO: check if gs is active atm

		m_gamestates.erase(i);
		Log(DBGTYPE) << "unregistered gamestate: " << gs->getName();
	}

	bool GameStateManager::gameStateExists(const std::string& name) const {
		return m_gamestates.find(name) != m_gamestates.end();
	}

	GameState* GameStateManager::getGameState(const std::string& name) const {
		type_gamestates::const_iterator i = m_gamestates.find(name);
		if (i == m_gamestates.end())
			throw NotFound("Gamestate not found: " + name);

		return i->second;
	}

	void GameStateManager::activate(const std::string& name) {
		GameState* gs = getGameState(name);
		m_gamestates_active.push_back(gs);
		gs->activate();
	}

	void GameStateManager::turn() {
		// needs a copy because if one gamestate activates/deactives another gamestate the iterators would be invalid
		type_gamestates_active active_copy = m_gamestates_active;
		std::for_each(active_copy.begin(), active_copy.end(), std::mem_fun(&GameState::turn));
	}

}//FIFE
