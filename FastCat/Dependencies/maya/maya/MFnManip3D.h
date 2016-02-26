#ifndef _MFnManip3D
#define _MFnManip3D
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
// CLASS:    MFnManip3D
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>

class MEulerRotation;

// ****************************************************************************
// CLASS DECLARATION (MFnManip3D)

//! \ingroup OpenMayaUI MFn
//! \brief 3D manipulator function set 
/*!

  MFnManip3D allows the creation and manipulation of 3D manipulators.
  MFnManip3D is the base class from which MFnFreePointTriadManip,
  MFnDirectionManip, MFnDistanceManip, MFnPointOnCurveManip,
  MFnPointOnSurfaceManip, MFnDiscManip, MFnCircleSweepManip,
  MFnToggleManip, MFnStateManip, and MFnCurveSegmentManip are derived.
*/
class OPENMAYAUI_EXPORT MFnManip3D : public MFnTransform
{
	declareDagMFn(MFnManip3D, MFnTransform);
public:
	bool 			isVisible(MStatus *ReturnStatus) const;
	MStatus			setVisible(bool isVisible);
	float			manipScale(MStatus *ReturnStatus) const;
	MStatus			setManipScale(float size);
	bool			isOptimizePlaybackOn(MStatus *ReturnStatus) const;
	MStatus			setOptimizePlayback(bool optimizePlayback);
	static float	globalSize();
	static void		setGlobalSize(float size);
	static float	handleSize();
	static void		setHandleSize(float size);
	static float	lineSize();
	static void		setLineSize(float size);
	static MStatus	deleteManipulator(MObject& manip);
	static bool		drawPlaneHandles();
	static void		setDrawPlaneHandles(bool drawPlaneHandles);
	MEulerRotation  rotateXYZValue(const unsigned int valIndex,MStatus *ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnManip3D, MFnTransform );

END_NO_SCRIPT_SUPPORT:

protected:

private:
};

#endif /* __cplusplus */
#endif /* _MFnManip3D */
