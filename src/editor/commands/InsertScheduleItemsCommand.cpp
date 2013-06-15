/*
 * Copyright 2006-2009, Stephan Aßmus <superstippi@gmx.de>
 * All rights reserved. Distributed under the terms of the GNU GPL v2.
 */

#include "InsertScheduleItemsCommand.h"

#include <new>
#include <stdio.h>

#include "common.h"

#include "Schedule.h"
#include "ScheduleItem.h"
#include "Selection.h"

using std::nothrow;

// constructor
InsertScheduleItemsCommand::InsertScheduleItemsCommand(Schedule* schedule,
	Selection* selection, ScheduleItem** const items, int32 count,
	uint64 insertFrame, int32 insertIndex)
	: Command()
	, fSchedule(schedule)
	, fSelection(selection)
	, fItems((items && count > 0) ? new (nothrow) ScheduleItem*[count]
	  							  : NULL)
	, fCount(count)
	, fInsertIndex(insertIndex)

	, fInsertFrame(insertFrame)

	, fItemsInserted(false)
{
	if (fItems) {
		memcpy(fItems, items, fCount * sizeof(ScheduleItem*));
	}
}

// destructor
InsertScheduleItemsCommand::~InsertScheduleItemsCommand()
{
	if (!fItemsInserted) {
		// the ScheduleItems belong to us
		for (int32 i = 0; i < fCount; i++)
			delete fItems[i];
	}
	delete[] fItems;
}

// InitCheck
status_t
InsertScheduleItemsCommand::InitCheck()
{
	if (fSchedule && fItems && fCount > 0)
		return B_OK;
	return B_NO_INIT;
}

// Perform
status_t
InsertScheduleItemsCommand::Perform()
{
	ScheduleNotificationBlock _(fSchedule);

	int32 insertIndex = fInsertIndex;
	if (insertIndex < 0)
		insertIndex = fSchedule->CountItems();

	// insert items
	uint64 insertFrame = fInsertFrame;
	for (int32 i = 0; i < fCount; i++) {
		// make sure the item is at the right place
		fItems[i]->FilterStartFrame(&insertFrame);
		fItems[i]->SetStartFrame(insertFrame);
		// add it
		if (!fSchedule->AddItem(fItems[i], insertIndex++)) {
			// ERROR - roll back, remove the items
			// we already added
			print_error("InsertScheduleItemsCommand::Perform() - "
							"no memory to add items to schedule!\n");
			for (int32 j = i - 1; j >= 0; j--)
				fSchedule->RemoveItem(fItems[j]);
			return B_NO_MEMORY;
		}
		insertFrame += fItems[i]->Duration();

		if (fSelection)
			fSelection->Select(fItems[i], i > 0);
	}
	fSchedule->SanitizeStartFrames();

	fItemsInserted = true;

	return B_OK;
}

// Undo
status_t
InsertScheduleItemsCommand::Undo()
{
	ScheduleNotificationBlock _(fSchedule);

	// remove items
	for (int32 i = fCount - 1; i >= 0; i--) {
		if (fSelection)
			fSelection->Deselect(fItems[i]);

		fSchedule->RemoveItem(fItems[i]);
	}
	fItemsInserted = false;

	fSchedule->SanitizeStartFrames();

	return B_OK;
}

// GetName
void
InsertScheduleItemsCommand::GetName(BString& name)
{
	if (fCount > 1)
		name << "Insert Schedule Items";
	else
		name << "Insert Schedule Item";
}
