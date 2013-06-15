/*
 * Copyright 2001-2010, Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "ViewState.h"

#include "StateView.h"

// constructor
ViewState::ViewState(StateView* view)
	: fView(view),
	  fMouseInfo(view->MouseInfo())
{
}

// constructor
ViewState::ViewState(const ViewState& other)
	: fView(other.fView),
	  fMouseInfo(other.fMouseInfo)
{
}

// destructor
ViewState::~ViewState()
{
}

// #pragma mark -

// Init
void
ViewState::Init()
{
}

// Cleanup
void
ViewState::Cleanup()
{
}

// #pragma mark -

// Draw
void
ViewState::Draw(BView* into, BRect updateRect)
{
}

// MessageReceived
bool
ViewState::MessageReceived(BMessage* message, Command** _command)
{
	return false;
}

// #pragma mark -

// MouseDown
void
ViewState::MouseDown(BPoint where, uint32 buttons, uint32 clicks)
{
}

// MouseMoved
void
ViewState::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
}

// MouseUp
Command*
ViewState::MouseUp()
{
	return NULL;
}

// #pragma mark -

// ModifiersChanged
void
ViewState::ModifiersChanged(uint32 modifiers)
{
}

// HandleKeyDown
bool
ViewState::HandleKeyDown(const StateView::KeyEvent& event,
						 Command** _command)
{
	return false;
}

// HandleKeyUp
bool
ViewState::HandleKeyUp(const StateView::KeyEvent& event,
					   Command** _command)
{
	return false;
}

// UpdateCursor
bool
ViewState::UpdateCursor()
{
	return false;
}

// Bounds
BRect
ViewState::Bounds() const
{
	return BRect(0, 0, -1, -1);
}
