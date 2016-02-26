#ifndef _MPxGeometryData
#define _MPxGeometryData
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MPxGeometryData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MPxData.h>
#include <maya/MMatrix.h>

// ****************************************************************************
// DECLARATIONS

class MPxGeometryIterator;
class MObjectArray;
class MIntArray;

// ****************************************************************************
// CLASS DECLARATION (MPxGeometryData)

//! \ingroup OpenMaya MPx
//! \brief Base Class for User-defined Dependency Graph Geometry Data Types. 
/*!

  This class is used to provide a wrapper or container for some
  arbitrary geometry type that you have defined. This allows your data
  type to exist as typed attributes on dependency nodes and to be
  passed through dependency graph connections.

  MP`xGeometryData is similar to MPxData but includes methods to
  support sets (also known as groups) and component iteration.

  For user defined shapes (MPxSurfaceShape derived classes), in order
  to support maya's deformers you must declare an MPxGeometryData
  class as well as a geometry iterator (MPxGeometryIterator).

  To register geometry data use MFnPlugin::registerData with the type
  argument equal to MPxData::kGeometryData.
*/
class OPENMAYA_EXPORT MPxGeometryData : public MPxData
{
public:
	MPxGeometryData();
	virtual ~MPxGeometryData();

	// Methods to provide a component iterator for the geometry
	//
	virtual MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents);

	virtual MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents,
											bool world) const;

	virtual bool	updateCompleteVertexGroup( MObject & component ) const;

	virtual bool	deleteComponent( const MObjectArray& compList );
	virtual bool	deleteComponentsFromGroups( const MObjectArray& compList,
												MIntArray& groupIdArray,
												MObjectArray& groupComponentArray );

	virtual void	smartCopy( const MPxGeometryData *srcGeom );
	virtual	void			copy( const MPxData& src ) = 0;

	virtual MTypeId         typeId() const = 0;
	virtual MString         name() const = 0;

	void			setMatrix(const MMatrix &);
	const MMatrix		&matrix() const;
	bool			matrix(MMatrix &) const;

protected:
// No protected members

private:
};

#endif /* __cplusplus */
#endif /* _MPxGeometryData */
