/***************************************************************************
 *   Copyright (C) 2005-2008 by the FIFE team                              *
 *   http://www.fifengine.de                                               *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
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

// Standard C++ library includes

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/structures/purge.h"
#include "model/metamodel/metamodel.h"
#include "model/metamodel/abstractpather.h"
#include "structures/map.h"
#include "util/base/exception.h"

#include "model.h"

namespace FIFE {

	Model::Model(): 
		FifeClass(), 
		m_meta(new MetaModel()),
		m_timeprovider(NULL) {
	}

	Model::~Model() {
		purge(m_maps);
		purge(m_pathers);
		delete m_meta;
	}

	Map* Model::createMap(const std::string& identifier) {
		std::vector<Map*>::const_iterator it = m_maps.begin();
		for(; it != m_maps.end(); ++it) {
			if(identifier == (*it)->Id())
				throw NameClash(identifier);
		}

		Map* map = new Map(identifier, &m_timeprovider);
		map->setTriggerController(m_triggercontroller);
		m_maps.push_back(map);
		return map;
	}

	void Model::adoptPather(AbstractPather* pather) {
		m_pathers.push_back(pather);
	}
	
	AbstractPather* Model::getPather(const std::string& pathername) {
		std::vector<AbstractPather*>::const_iterator it = m_pathers.begin();
		for(; it != m_pathers.end(); ++it) {
			if ((*it)->getName() == pathername) {
				return *it;
			}
		}
		return NULL;
	}
	
	std::list<Map*> Model::getMaps() const {
		std::list<Map*> lst;

		std::vector<Map*>::const_iterator it = m_maps.begin();
		for(; it != m_maps.end(); ++it) {
			lst.push_back(*it);
		}

		return lst;
	}

	std::list<Map*> Model::getMaps(const std::string& field, const std::string& value) const {
		std::list<Map*> matches;

		std::vector<Map*>::const_iterator it = m_maps.begin();
		for(; it != m_maps.end(); ++it) {
			if((*it)->get(field) == value)
				matches.push_back(*it);
		}

		return matches;
	}

	void Model::deleteMap(Map* map) {
		std::vector<Map*>::iterator it = m_maps.begin();
		for(; it != m_maps.end(); ++it) {
			if(*it == map) {
				delete *it;
				m_maps.erase(it);
				return ;
			}
		}
	}

	size_t Model::getNumMaps() const {
		return m_maps.size();
	}

	void Model::deleteMaps() {
		purge(m_maps);
		m_maps.clear();
	}

	MetaModel* Model::getMetaModel() {
		return m_meta;
	}

	void Model::update() {
		std::vector<Map*>::iterator it = m_maps.begin();
		for(; it != m_maps.end(); ++it) {
			(*it)->update();
		}
		std::vector<AbstractPather*>::iterator jt = m_pathers.begin();
		for(; jt != m_pathers.end(); ++jt) {
			(*jt)->update();
		}
	}

	void Model::setTriggerController(ITriggerController* triggercontroller){
		m_triggercontroller = triggercontroller;
	}
	
} //FIFE

