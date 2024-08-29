// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- TTRH.h : Declaration of the TTRH
//-----------------------------------------------------------------------------
#pragma once

#ifdef TETRAHEDRONDBX_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include <arxHeaders.h>

//-----------------------------------------------------------------------------
class DLLIMPEXP TTRH : public AcDbEntity {

private:
	class Edge : public AcGePoint3dArray
	{
	public:
		Edge();
		Edge(const Edge&);
		~Edge() {}

	public:
		AcGePoint3d m_fVer;
		AcGePoint3d m_sVer;
		AcGePoint3d m_tVer;
		AcGePoint3dArray m_arEdge;

		double areaEdge(const AcGePoint3dArray&);
	};

public:
	ACRX_DECLARE_MEMBERS(TTRH) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TTRH () ;
	TTRH (AcGePoint3d, AcGePoint3d, AcGePoint3d, AcGePoint3d);
	TTRH(const TTRH&);
	virtual ~TTRH () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual void subViewportDraw (AcGiViewportDraw *mode) ;
	virtual Adesk::UInt32 subViewportDrawLogicalFlags (AcGiViewportDraw *vd) ;
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;

	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d &insertionMat) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subGetGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const;
	void storePoints();
	double volumeTTRH(const AcGePoint3d&, const AcGePoint3d&, const AcGePoint3d&, const AcGePoint3d&);

public:
	AcGePoint3d m_firstVertex;
	AcGePoint3d m_secondVertex;
	AcGePoint3d m_thirdVertex;
	AcGePoint3d m_fourthVertex;
	
	Edge m_firstEdge;
	Edge m_secondEdge;
	Edge m_thirdEdge;
	Edge m_fourthEdge;
	
	AcGePoint3d m_centreOfInscribedSphere;
	double m_radiusoOfInscribedSphere;
	AcGeVector3dArray m_arNormVecOfInscribedSphere;

	Adesk::Int32 m_nFirstEdge;
	Adesk::Int32 m_nSecondEdge;
	Adesk::Int32 m_nThirdEdge;
	Adesk::Int32 m_nFourthEdge;
	Adesk::Int32 m_nNormVec;
} ;

#ifdef TETRAHEDRONDBX_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TTRH)
#endif
