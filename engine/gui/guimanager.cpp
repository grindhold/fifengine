/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
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
#include <guichan/sdl/sdlinput.hpp>
#include <guichan/focushandler.hpp>
#include <guichan.hpp>

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "video/renderbackend.h"
#include "video/rendermanager.h"
#include "video/screen.h"
#include "video/gui/gcnimageloader.h"
#include "util/settingsmanager.h"
#include "util/debugutils.h"

#include "guimanager.h"
#include "console.h"
#include "font.h"


namespace FIFE {

	GUIManager::GUIManager() : 
		m_gcn_gui(new gcn::Gui()), 
		m_focushandler(0),
        m_gcn_topcontainer(new gcn::Container()), 
        m_gcn_imgloader(new GCNImageLoader()) , 
        m_input(new gcn::SDLInput()) {
		m_font = 0;
		m_console = 0;

		m_gcn_gui->setTop(m_gcn_topcontainer);
		m_gcn_topcontainer->setOpaque(false);
		m_gcn_gui->setInput(m_input);

		gcn::Image::setImageLoader(m_gcn_imgloader);
		m_focushandler = m_gcn_topcontainer->_getFocusHandler();
	}

	GUIManager::~GUIManager() {
		delete m_console;
		delete m_gcn_topcontainer;
		delete m_gcn_imgloader;
		delete m_input;
		delete m_gcn_gui;
		delete m_font;
	}

	void GUIManager::onSdlEvent(SDL_Event& evt) {
		gcn::SDLInput *input = dynamic_cast<gcn::SDLInput*>(m_gcn_gui->getInput());
		if (!input) {
			Warn("GUIManager")
				<< "GuichanGUI->getInput == 0 ... discarding events!";
			return;
		}
		input->pushInput(evt);
	}

	void GUIManager::resizeTopContainer(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
		m_gcn_topcontainer->setDimension(gcn::Rectangle(x, y, width, height));
	}

	gcn::Gui* GUIManager::getGuichanGUI() const {
		return m_gcn_gui;
	}

	void GUIManager::add(gcn::Widget* widget) {
		if( !m_widgets.count(widget) ) {
			m_gcn_topcontainer->add(widget);
			m_widgets.insert(widget);
		}
	}

	void GUIManager::remove(gcn::Widget* widget) {
		if( m_widgets.count(widget) ) {
			m_widgets.erase(widget);
			m_gcn_topcontainer->remove(widget);
		}
	}

	void GUIManager::init() {
		Screen* screen = CRenderBackend()->getMainScreen();
		m_gcn_gui->setGraphics(screen);
		resizeTopContainer(0, 0, screen->getWidth(), screen->getHeight());

		SettingsManager* settings = SettingsManager::instance();
		std::string fontfile = settings->read<std::string>("GUIFont", "content/fonts/FreeMono.ttf");
		unsigned int fontsize = settings->read<unsigned int>("GUIFontSize", 16);
		std::string glyphs = settings->read<std::string>("GUIFontGlyphs",
			" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
			".,!?-+/:();%`'*#=[]");
		m_font = FontBase::load(fontfile, fontsize,glyphs);
		gcn::Widget::setGlobalFont(m_font);

		m_console = new Console();

// 		gcn::Icon* testicon = new gcn::Icon(new gcn::Image("art/intrface/about.frm"));
// 		testicon->setPosition(screen->getWidth() -  testicon->getWidth() - 10, 10);
// 		add(testicon);

	}

	void GUIManager::setGlobalFont(gcn::Font* font) {
		if( font == 0) {
			gcn::Widget::setGlobalFont(m_font);
		} else {
			gcn::Widget::setGlobalFont(font);
		}
	}

	void GUIManager::turn() {
		m_gcn_gui->logic();
		m_gcn_gui->draw();
	}

	void GUIManager::evaluateKeyEventConsumption(IKeyEvent& evt) {
		gcn::Widget* w = m_focushandler->getFocused();
		if (w) {
			evt.consume();
			//std::cout << "key event consumed" << std::endl;
		}
	}

	void GUIManager::evaluateMouseEventConsumption(IMouseEvent& evt) {
		gcn::Widget* w = m_gcn_topcontainer->getWidgetAt(evt.getX(), evt.getY());
		if (w && w->isVisible()) {
			evt.consume();
			//std::cout << "mouse event consumed" << std::endl;
		}
	}

	void GUIManager::mousePressed(IMouseEvent& evt) {
 		evaluateMouseEventConsumption(evt);
		if (!evt.isConsumed()) {
			m_focushandler->focusNone();
		}
	}
}