/*
 * Copyright 2006-2007, Ingo Weinhold <ingo_weinhold@gmx.de>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "Debug.h"
#include "Referenceable.h"

// constructor
Referenceable::Referenceable(bool deleteWhenUnreferenced)
	: fReferenceCount(1),
	  fDeleteWhenUnreferenced(deleteWhenUnreferenced)
{
}

// destructor
Referenceable::~Referenceable()
{
}

// AddReference
void
Referenceable::AddReference()
{
	atomic_add(&fReferenceCount, 1);
}

// RemoveReference
bool
Referenceable::RemoveReference()
{
	bool unreferenced = (atomic_add(&fReferenceCount, -1) == 1);
	if (fDeleteWhenUnreferenced && unreferenced)
		delete this;
	return unreferenced;
}

// CountReferences
int32
Referenceable::CountReferences() const
{
	return fReferenceCount;
}

