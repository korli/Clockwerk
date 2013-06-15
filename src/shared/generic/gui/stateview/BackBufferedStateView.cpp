/*
 * Copyright 2001-2009, Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "BackBufferedStateView.h"

#include <new>
#include <stdio.h>

#include <Bitmap.h>
#include <Region.h>

using std::nothrow;

// constructor
BackBufferedStateView::BackBufferedStateView(BRect frame, const char* name,
		uint32 resizingMode, uint32 flags)
	: StateView(frame, name, resizingMode, flags | B_FRAME_EVENTS)
	, fOffscreenBitmap(NULL)
	, fOffscreenView(NULL)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	_AllocBackBitmap(frame.Width(), frame.Height());
}

// destructor
BackBufferedStateView::~BackBufferedStateView()
{
	_FreeBackBitmap();
}

// #pragma mark -

// AttachedToWindow
void
BackBufferedStateView::AttachedToWindow()
{
	SyncGraphicsState();
	StateView::AttachedToWindow();
}

// FrameResized
void
BackBufferedStateView::FrameResized(float width, float height)
{
	_AllocBackBitmap(width, height);
}

// Draw
void
BackBufferedStateView::Draw(BRect updateRect)
{
	if (!fOffscreenView) {
		DrawInto(this, updateRect);
	} else {
		BPoint boundsLeftTop = Bounds().LeftTop();
		if (fOffscreenBitmap->Lock()) {
			fOffscreenView->PushState();

			// apply scrolling offset to offscreen view
			fOffscreenView->SetOrigin(-boundsLeftTop.x, -boundsLeftTop.y);
			// mirror the clipping of this view
			// to the offscreen view for performance
			BRegion clipping;
			GetClippingRegion(&clipping);
			fOffscreenView->ConstrainClippingRegion(&clipping);
	
			DrawBackgroundInto(fOffscreenView, updateRect);
			DrawInto(fOffscreenView, updateRect);

			fOffscreenView->PopState();
			fOffscreenView->Sync();

			fOffscreenBitmap->Unlock();
		}
		// compensate scrolling offset in BView
		BRect bitmapRect = updateRect;
		bitmapRect.OffsetBy(-boundsLeftTop.x, -boundsLeftTop.y);

		SetDrawingMode(B_OP_COPY);
		DrawBitmap(fOffscreenBitmap, bitmapRect, updateRect);
	}
}

// #pragma mark -

// DrawBackgroundInto
void
BackBufferedStateView::DrawBackgroundInto(BView* view, BRect updateRect)
{
	view->FillRect(updateRect, B_SOLID_LOW);
}

// DrawInto
void
BackBufferedStateView::DrawInto(BView* view, BRect updateRect)
{
	StateView::Draw(view, updateRect);
}

// SyncGraphicsState
void
BackBufferedStateView::SyncGraphicsState()
{
	if (!fOffscreenView || !fOffscreenView->LockLooper())
		return;

	BFont font;
	GetFont(&font);
	fOffscreenView->SetFont(&font);

	fOffscreenView->SetLowColor(LowColor());
	fOffscreenView->SetHighColor(HighColor());
	fOffscreenView->SetDrawingMode(DrawingMode());

	source_alpha srcAlpha;
	alpha_function alphaFunc;
	GetBlendingMode(&srcAlpha, &alphaFunc);
	fOffscreenView->SetBlendingMode(srcAlpha, alphaFunc);

// NOTE: seems to be the cause for some "lock ups", app_server
// doesn't send some data back to the client, the client waits
// forever
//	fOffscreenView->SetLineMode(LineCapMode(), LineJoinMode(),
//		LineMiterLimit());

	fOffscreenView->Sync();

	fOffscreenView->UnlockLooper();
}

// #pragma mark -

// _AllocBackBitmap
void
BackBufferedStateView::_AllocBackBitmap(float width, float height)
{
	// sanity check
	if (width <= 0.0 || height <= 0.0)
		return;

	if (fOffscreenBitmap) {
		// see if the bitmap needs to be expanded
		BRect b = fOffscreenBitmap->Bounds();
		if (b.Width() >= width && b.Height() >= height)
			return;

		// it does, clean up
		_FreeBackBitmap();
	}

	BRect b(0.0, 0.0, width, height);
	fOffscreenBitmap = new (nothrow) BBitmap(b, B_RGB32, true);
	if (!fOffscreenBitmap) {
		fprintf(stderr, "BackBufferedStateView::_AllocBackBitmap(): "
			"failed to allocate\n");
		return;
	}
	if (fOffscreenBitmap->IsValid()) {
		fOffscreenView = new BView(b, 0, B_FOLLOW_NONE, B_WILL_DRAW);
		fOffscreenBitmap->AddChild(fOffscreenView);
		SyncGraphicsState();
	} else {
		_FreeBackBitmap();
		fprintf(stderr, "BackBufferedStateView::_AllocBackBitmap(): "
			"bitmap invalid\n");
	}
}

// _FreeBackBitmap
void
BackBufferedStateView::_FreeBackBitmap()
{
	if (fOffscreenBitmap) {
		delete fOffscreenBitmap;
		fOffscreenBitmap = NULL;
		fOffscreenView = NULL;
	}
}

