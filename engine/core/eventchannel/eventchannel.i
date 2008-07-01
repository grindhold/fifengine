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

%module fife
%{
#include "eventchannel/base/ec_event.h"
#include "eventchannel/base/ec_inputevent.h"
#include "eventchannel/command/ec_command.h"
#include "eventchannel/command/ec_commandids.h"
#include "eventchannel/command/ec_icommandlistener.h"
#include "eventchannel/key/ec_key.h"
#include "eventchannel/key/ec_keyevent.h"
#include "eventchannel/key/ec_ikeylistener.h"
#include "eventchannel/source/ec_eventsourcetypes.h"
#include "eventchannel/source/ec_ieventsource.h"
#include "eventchannel/mouse/ec_mouseevent.h"
#include "eventchannel/mouse/ec_imouselistener.h"
#include "eventchannel/widget/ec_widgetevent.h"
#include "eventchannel/widget/ec_iwidgetlistener.h"
#include "eventchannel/eventmanager.h"
%}

%include "eventchannel/key/ec_key.h"
%include "eventchannel/source/ec_eventsourcetypes.h"
%include "eventchannel/command/ec_commandids.h"

namespace FIFE {
	%feature("director") IEventSource;
	class IEventSource {
	public:
		virtual EventSourceType getEventSourceType() = 0;
		virtual ~IEventSource();
	};

	class Event {
	public:
		virtual void consume();
		virtual bool isConsumed() const;
		virtual IEventSource* getSource();
		virtual gcn::Widget* getSourceWidget();
		virtual int getTimeStamp() const;
		virtual std::string getDebugString() const;
		virtual const std::string& getName() const;
		virtual ~IEvent() {}
	private:
		Event();
	};

	class InputEvent: public Event {
	public:
		virtual bool isConsumedByGuichan() const;
		virtual bool isAltPressed() const;
		virtual bool isControlPressed() const;
		virtual bool isMetaPressed() const;
		virtual bool isShiftPressed() const;
		virtual ~IInputEvent() {}
	private:
		InputEvent();
	};

	class Command: public Event {
	public:
		Command();
		virtual ~Command();

		CommandType getCommandType();
		void setCommandType(CommandType type);

		int getCode();
		void setCode(int code);

		virtual void setSource(IEventSource* source);
		virtual void setSourceWidget(gcn::Widget* widget);
		virtual void setTimeStamp(int timestamp);
	};

	%feature("director") ICommandListener;
	class ICommandListener {
	public:
		virtual void onCommand(Command& command) = 0;
		virtual ~ICommandListener() {}
	};

	class KeyEvent: public InputEvent  {
	public:
		enum KeyEventType {
			UNKNOWN = -1,
			PRESSED = 0,
			RELEASED
		};
		virtual KeyEventType getType() const;
		virtual bool isNumericPad() const;
		virtual const Key& getKey() const;
		virtual ~KeyEvent();
	private:
		KeyEvent();
	};

	%feature("director") IKeyListener;
	class IKeyListener {
	public:
		virtual void keyPressed(KeyEvent& evt) = 0;
		virtual void keyReleased(KeyEvent& evt) = 0;
		virtual ~IKeyListener();
	};

	class MouseEvent: public InputEvent {
	public:
		enum MouseEventType {
			UNKNOWN_EVENT = -1,
			MOVED = 0,
			PRESSED,
			RELEASED,
			WHEEL_MOVED_DOWN,
			WHEEL_MOVED_UP,
			CLICKED,
			ENTERED,
			EXITED,
			DRAGGED
		};

		enum MouseButtonType {
			EMPTY = 0,
			LEFT = 1,
			RIGHT = 2,
			MIDDLE = 4,
			UNKNOWN_BUTTON = 8
		};
		virtual int getX() const;
		virtual int getY() const;
		virtual MouseEventType getType() const;
		virtual MouseButtonType getButton() const;
		virtual ~IMouseEvent();
	private:
		MouseEvent();
	};

	%feature("director") IMouseListener;
	class IMouseListener {
	public:
		virtual void mouseEntered(MouseEvent& evt) = 0;
		virtual void mouseExited(MouseEvent& evt) = 0;
		virtual void mousePressed(MouseEvent& evt) = 0;
		virtual void mouseReleased(MouseEvent& evt) = 0;
		virtual void mouseClicked(MouseEvent& evt) = 0;
		virtual void mouseWheelMovedUp(MouseEvent& evt) = 0;
		virtual void mouseWheelMovedDown(MouseEvent& evt) = 0;
		virtual void mouseMoved(MouseEvent& evt) = 0;
		virtual void mouseDragged(MouseEvent& evt) = 0;
		virtual ~IMouseListener();
	};

	class WidgetEvent: public Event {
	public:
		virtual const std::string& getId();
		virtual ~WidgetEvent();
	private:
		WidgetEvent();
	};

	%feature("director") IWidgetListener;
	class IWidgetListener {
	public:
		virtual void onWidgetAction(WidgetEvent& evt) = 0;
		virtual ~IWidgetListener();
	};

	class EventManager {
	public:
		EventManager();
		virtual ~EventManager();
		void addCommandListener(ICommandListener* listener);
		void removeCommandListener(ICommandListener* listener);
		void addKeyListener(IKeyListener* listener);
		void removeKeyListener(IKeyListener* listener);
		void addMouseListener(IMouseListener* listener);
		void removeMouseListener(IMouseListener* listener);
		void addWidgetListener(IWidgetListener* listener);
		void removeWidgetListener(IWidgetListener* listener);
		EventSourceType getEventSourceType();
		void dispatchCommand(Command& command);
		void setNonConsumableKeys(const std::vector<int>& keys);
		std::vector<int> getNonConsumableKeys();
	};
};
