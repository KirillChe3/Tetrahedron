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
//----- TTRH.cpp : Implementation of TTRH
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TTRH.h"
#include <cmath>

//-----------------------------------------------------------------------------
Adesk::UInt32 TTRH::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	TTRH, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, TTRH,
ADSKTETRAHEDRONDBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
TTRH::TTRH () : AcDbEntity(), m_firstVertex{ AcGePoint3d(0,0,0) }, m_secondVertex{ AcGePoint3d(50,0,10) }, m_thirdVertex{ AcGePoint3d(30,-50,30) }, m_fourthVertex{ AcGePoint3d(20,100,50) } { 
	storePoints();
}

TTRH::TTRH (AcGePoint3d firVrt, AcGePoint3d secVrt, AcGePoint3d thrVrt, AcGePoint3d frtVrt) : AcDbEntity(), m_firstVertex{ firVrt }, m_secondVertex{ secVrt }, m_thirdVertex{ thrVrt }, m_fourthVertex{ frtVrt }
{
	storePoints();
}

TTRH::TTRH(const TTRH& other)
{

	m_firstVertex = other.m_firstVertex;
	m_secondVertex = other.m_secondVertex;
	m_thirdVertex = other.m_thirdVertex;
	m_fourthVertex = other.m_fourthVertex;
	m_firstEdge = other.m_firstEdge;
	m_secondEdge = other.m_secondEdge;
	m_thirdEdge = other.m_thirdEdge;
	m_fourthEdge = other.m_fourthEdge;
	m_nFirstEdge = other.m_nFirstEdge;
	m_nSecondEdge = other.m_nSecondEdge;
	m_nThirdEdge = other.m_nThirdEdge;
	m_nFourthEdge = other.m_nFourthEdge;

	m_centreOfInscribedSphere = other.m_centreOfInscribedSphere;
	m_radiusoOfInscribedSphere = other.m_radiusoOfInscribedSphere;
	m_arNormVecOfInscribedSphere = other.m_arNormVecOfInscribedSphere;
	m_nNormVec = other.m_nNormVec;
}

TTRH::~TTRH ()
{
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus TTRH::dwgOutFields (AcDbDwgFiler *pFiler) const { // TO DO Modify with rad, cent
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (TTRH::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(m_firstVertex);
	pFiler->writeItem(m_secondVertex);
	pFiler->writeItem(m_thirdVertex);
	pFiler->writeItem(m_fourthVertex);
	pFiler->writeItem(m_centreOfInscribedSphere);
	pFiler->writeItem(m_radiusoOfInscribedSphere);

	Adesk::Int32 aNum1 = static_cast<Adesk::Int32>(m_nFirstEdge);
	pFiler->writeItem(aNum1);

	Adesk::Int32 aNum2 = static_cast<Adesk::Int32>(m_nSecondEdge);
	pFiler->writeItem(aNum2);

	Adesk::Int32 aNum3 = static_cast<Adesk::Int32>(m_nThirdEdge);
	pFiler->writeItem(aNum3);

	Adesk::Int32 aNum4 = static_cast<Adesk::Int32>(m_nFourthEdge);
	pFiler->writeItem(aNum4);

	Adesk::Int32 aNum5 = static_cast<Adesk::Int32>(m_nNormVec);
	pFiler->writeItem(aNum5);

	if (m_nFirstEdge > 0)
		for (size_t i = 0; i < m_nFirstEdge; i++)
			pFiler->writeItem(m_firstEdge.at(i));

	if (m_nSecondEdge > 0)
		for (size_t i = 0; i < m_nSecondEdge; i++)
			pFiler->writeItem(m_secondEdge.at(i));

	if (m_nThirdEdge > 0)
		for (size_t i = 0; i < m_nThirdEdge; i++)
			pFiler->writeItem(m_thirdEdge.at(i));

	if (m_nFourthEdge > 0)
		for (size_t i = 0; i < m_nFourthEdge; i++)
			pFiler->writeItem(m_fourthEdge.at(i));

	if (m_nFourthEdge > 0)
		for (size_t i = 0; i < m_nNormVec; i++)
			pFiler->writeItem(m_arNormVecOfInscribedSphere.at(i));

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus TTRH::dwgInFields (AcDbDwgFiler *pFiler) { // TO DO Modify with rad, cent
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > TTRH::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < TTRH::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readItem(&m_firstVertex);
	pFiler->readItem(&m_secondVertex);
	pFiler->readItem(&m_thirdVertex);
	pFiler->readItem(&m_fourthVertex);
	pFiler->readItem(&m_centreOfInscribedSphere);
	pFiler->readItem(&m_radiusoOfInscribedSphere);

	AcGePoint3d pti;
	AcGeVector3d vec;
	Adesk::Int32 aNum1, aNum2, aNum3, aNum4, aNum5;
	
	pFiler->readItem(&aNum1);

	m_nFirstEdge = static_cast<int>(aNum1);

	if (m_nFirstEdge > 0)
	{
		m_firstEdge.removeAll();
		for (size_t i = 0; i < m_nFirstEdge; i++)
		{
			pFiler->readItem(&pti);
			m_firstEdge.append(pti);
		}
	}

	pFiler->readItem(&aNum2);

	m_nSecondEdge = static_cast<int>(aNum2);

	if (m_nSecondEdge > 0)
	{
		m_secondEdge.removeAll();
		for (size_t i = 0; i < m_nSecondEdge; i++)
		{
			pFiler->readItem(&pti);
			m_secondEdge.append(pti);
		}
	}

	pFiler->readItem(&aNum3);

	m_nThirdEdge = static_cast<int>(aNum3);

	if (m_nThirdEdge > 0)
	{
		m_thirdEdge.removeAll();
		for (size_t i = 0; i < m_nThirdEdge; i++)
		{
			pFiler->readItem(&pti);
			m_thirdEdge.append(pti);
		}
	}

	pFiler->readItem(&aNum4);

	m_nFourthEdge = static_cast<int>(aNum4);

	if (m_nFourthEdge > 0)
	{
		m_fourthEdge.removeAll();
		for (size_t i = 0; i < m_nFourthEdge; i++)
		{
			pFiler->readItem(&pti);
			m_fourthEdge.append(pti);
		}
	}

	pFiler->readItem(&aNum5);

	m_nNormVec = static_cast<int>(aNum5);

	if (m_nNormVec > 0)
	{
		m_arNormVecOfInscribedSphere.removeAll();
		for (size_t i = 0; i < m_nNormVec; i++)
		{
			pFiler->readItem(&vec);
			m_arNormVecOfInscribedSphere.append(vec);
		}
	}
	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus TTRH::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("TTRH")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, TTRH::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus TTRH::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("TTRH")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > TTRH::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < TTRH::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch ( rb.restype ) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....

			default:
				//----- An unrecognized group. Push it back so that the subclass can read it again.
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean TTRH::subWorldDraw (AcGiWorldDraw *mode) { // TO DO Modify with rad, cent
	assertReadEnabled();

	Acad::ErrorStatus errSt;
	AcGeVector3d vec1, vec2;

	mode->geometry().polygon(m_nFirstEdge, m_firstEdge.asArrayPtr());
	mode->geometry().polygon(m_nSecondEdge, m_secondEdge.asArrayPtr());
	mode->geometry().polygon(m_nThirdEdge, m_thirdEdge.asArrayPtr());
	mode->geometry().polygon(m_nFourthEdge, m_fourthEdge.asArrayPtr());

	for (int i = 0; i < m_nNormVec; i++) {

	}
	
	vec1 = m_secondVertex - m_firstVertex;
	vec2 = m_thirdVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere[0] = vec1.crossProduct(vec2);
	mode->geometry().circle(m_centreOfInscribedSphere, m_radiusoOfInscribedSphere, m_arNormVecOfInscribedSphere[0]);
	
	vec1 = m_fourthVertex - m_thirdVertex;
	vec2 = m_secondVertex - m_thirdVertex;
	m_arNormVecOfInscribedSphere[1] = vec1.crossProduct(vec2);
	mode->geometry().circle(m_centreOfInscribedSphere, m_radiusoOfInscribedSphere, m_arNormVecOfInscribedSphere[1]);

	vec1 = m_secondVertex - m_firstVertex;
	vec2 = m_fourthVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere[2] = vec1.crossProduct(vec2);
	mode->geometry().circle(m_centreOfInscribedSphere, m_radiusoOfInscribedSphere, m_arNormVecOfInscribedSphere[2]);

	vec1 = m_thirdVertex - m_firstVertex;
	vec2 = m_fourthVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere[3] = vec1.crossProduct(vec2);
	mode->geometry().circle(m_centreOfInscribedSphere, m_radiusoOfInscribedSphere, m_arNormVecOfInscribedSphere[3]);

	//------ Returning Adesk::kFalse here will force viewportDraw() call
	return (Adesk::kFalse);
}

void TTRH::subViewportDraw (AcGiViewportDraw *mode) {
	assertReadEnabled () ;
	AcDbEntity::subViewportDraw (mode) ;
}

Adesk::UInt32 TTRH::subViewportDrawLogicalFlags (AcGiViewportDraw *vd) {
	assertReadEnabled () ;
	return (AcDbEntity::subViewportDrawLogicalFlags (vd)) ;
}

Adesk::UInt32 TTRH::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

	//- Osnap points protocol
Acad::ErrorStatus TTRH::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

Acad::ErrorStatus TTRH::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds,
	const AcGeMatrix3d &insertionMat) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
}

//- Grip points protocol
Acad::ErrorStatus TTRH::subGetGripPoints ( 
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)
	gripPoints.append(m_firstEdge[0]);
	gripPoints.append(m_firstEdge[1]);
	gripPoints.append(m_firstEdge[2]);
	gripPoints.append(m_fourthEdge[2]);

	return Acad::eOk;
	//return (AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds)) ;
}

Acad::ErrorStatus TTRH::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled();

	AcGeVector3d vec1, vec2;

	for (int i = 0; i < indices.length(); i++) {
		int idx = indices.at(i);
		if (idx == 0)
		{
			m_firstEdge[0] += offset;
			m_thirdEdge[0] += offset;
			m_fourthEdge[0] += offset;
		}

		if (idx == 1)
		{
			m_firstEdge[1] += offset;
			m_secondEdge[0] += offset;
			m_fourthEdge[1] += offset;
		}

		if (idx == 2)
		{
			m_firstEdge[2] += offset;
			m_secondEdge[1] += offset;
			m_thirdEdge[1] += offset;
		}
		
		if (idx == 3)
		{
			m_secondEdge[2] += offset;
			m_thirdEdge[2] += offset;
			m_fourthEdge[2] += offset;
		}
		m_centreOfInscribedSphere = (m_secondEdge.areaEdge(m_secondEdge) * m_firstEdge[0] + m_thirdEdge.areaEdge(m_thirdEdge) * m_firstEdge[1].asVector() + m_fourthEdge.areaEdge(m_fourthEdge) * m_firstEdge[2].asVector() + m_firstEdge.areaEdge(m_firstEdge) * m_secondEdge[2].asVector()) / (m_firstEdge.areaEdge(m_firstEdge) + m_secondEdge.areaEdge(m_secondEdge) + m_thirdEdge.areaEdge(m_thirdEdge) + m_fourthEdge.areaEdge(m_fourthEdge));

		m_radiusoOfInscribedSphere = 3.0 * volumeTTRH(m_firstEdge[0], m_firstEdge[1], m_firstEdge[2], m_secondEdge[2]) / (m_firstEdge.areaEdge(m_firstEdge) + m_secondEdge.areaEdge(m_secondEdge) + m_thirdEdge.areaEdge(m_thirdEdge) + m_fourthEdge.areaEdge(m_fourthEdge));

		vec1 = m_firstEdge[1] - m_firstEdge[0];
		vec2 = m_firstEdge[2] - m_firstEdge[0];
		m_arNormVecOfInscribedSphere[0] = vec1.crossProduct(vec2);

		vec1 = m_secondEdge[2] - m_secondEdge[1];
		vec2 = m_secondEdge[0] - m_secondEdge[1];
		m_arNormVecOfInscribedSphere[1] = vec1.crossProduct(vec2);

		vec1 = m_fourthEdge[1] - m_fourthEdge[0];
		vec2 = m_fourthEdge[2] - m_fourthEdge[0];
		m_arNormVecOfInscribedSphere[2] = vec1.crossProduct(vec2);

		vec1 = m_thirdEdge[1] - m_thirdEdge[0];
		vec2 = m_thirdEdge[2] - m_thirdEdge[0];
		m_arNormVecOfInscribedSphere[3] = vec1.crossProduct(vec2);
	}

	return (Acad::eOk);
	//assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	//return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

Acad::ErrorStatus TTRH::subGetGripPoints (
	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	const AcGeVector3d &curViewDir, const int bitflags
) const {
	assertReadEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}

Acad::ErrorStatus TTRH::subMoveGripPointsAt (
	const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
	const int bitflags
) {
	assertWriteEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
}

Acad::ErrorStatus TTRH::subTransformBy(const AcGeMatrix3d& xform) // TO DO Modify with rad, cent
{
	assertWriteEnabled();

	for (int i = 0; i < m_nFirstEdge; i++) {
		m_firstEdge[i].transformBy(xform);
	}

	for (int i = 0; i < m_nSecondEdge; i++) {
		m_secondEdge[i].transformBy(xform);
	}

	for (int i = 0; i < m_nThirdEdge; i++) {
		m_thirdEdge[i].transformBy(xform);
	}

	for (int i = 0; i < m_nFourthEdge; i++) {
		m_fourthEdge[i].transformBy(xform);
	}

	m_centreOfInscribedSphere.transformBy(xform);

	m_radiusoOfInscribedSphere = 3.0 * volumeTTRH(m_firstVertex, m_secondVertex, m_thirdVertex, m_fourthVertex) / (m_firstEdge.areaEdge(m_firstEdge) + m_secondEdge.areaEdge(m_secondEdge) + m_thirdEdge.areaEdge(m_thirdEdge) + m_fourthEdge.areaEdge(m_fourthEdge));

	for (int i = 0; i < m_nNormVec; i++) {
		m_arNormVecOfInscribedSphere[i].transformBy(xform);
	}
	
	return Acad::ErrorStatus();
}

Acad::ErrorStatus TTRH::subExplode(AcDbVoidPtrArray& entitySet) const
{
	return Acad::ErrorStatus();
}

void TTRH::storePoints()
{
	AcGeVector3d vec1, vec2;

	if (m_firstEdge.length() > 0) m_firstEdge.removeAll();
	m_firstEdge.append(m_firstVertex);
	m_firstEdge.append(m_secondVertex);
	m_firstEdge.append(m_thirdVertex);
	
	if (m_secondEdge.length() > 0) m_secondEdge.removeAll();
	m_secondEdge.append(m_secondVertex);
	m_secondEdge.append(m_thirdVertex);
	m_secondEdge.append(m_fourthVertex);
	
	if (m_thirdEdge.length() > 0) m_firstEdge.removeAll();
	m_thirdEdge.append(m_firstVertex);
	m_thirdEdge.append(m_thirdVertex);
	m_thirdEdge.append(m_fourthVertex);

	if (m_fourthEdge.length() > 0) m_firstEdge.removeAll();
	m_fourthEdge.append(m_firstVertex);
	m_fourthEdge.append(m_secondVertex);
	m_fourthEdge.append(m_fourthVertex);

	m_nFirstEdge = m_firstEdge.length();
	m_nSecondEdge = m_secondEdge.length();
	m_nThirdEdge = m_thirdEdge.length();
	m_nFourthEdge = m_fourthEdge.length();

	m_centreOfInscribedSphere = (m_secondEdge.areaEdge(m_secondEdge) * m_firstVertex + m_thirdEdge.areaEdge(m_thirdEdge) * m_secondVertex.asVector() + m_fourthEdge.areaEdge(m_fourthEdge) * m_thirdVertex.asVector() + m_firstEdge.areaEdge(m_firstEdge)*m_fourthVertex.asVector()) / (m_firstEdge.areaEdge(m_firstEdge) + m_secondEdge.areaEdge(m_secondEdge) + m_thirdEdge.areaEdge(m_thirdEdge) + m_fourthEdge.areaEdge(m_fourthEdge));

	m_radiusoOfInscribedSphere = 3.0 * volumeTTRH(m_firstVertex, m_secondVertex, m_thirdVertex, m_fourthVertex) / (m_firstEdge.areaEdge(m_firstEdge) + m_secondEdge.areaEdge(m_secondEdge) + m_thirdEdge.areaEdge(m_thirdEdge) + m_fourthEdge.areaEdge(m_fourthEdge));

	vec1 = m_secondVertex - m_firstVertex;
	vec2 = m_thirdVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere.append(vec1.crossProduct(vec2));
	
	vec1 = m_fourthVertex - m_thirdVertex;
	vec2 = m_secondVertex - m_thirdVertex;
	m_arNormVecOfInscribedSphere.append(vec1.crossProduct(vec2));
	
	vec1 = m_secondVertex - m_firstVertex;
	vec2 = m_fourthVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere.append(vec1.crossProduct(vec2));

	vec1 = m_thirdVertex - m_firstVertex;
	vec2 = m_fourthVertex - m_firstVertex;
	m_arNormVecOfInscribedSphere.append(vec1.crossProduct(vec2));

	m_nNormVec = m_arNormVecOfInscribedSphere.length();
}

TTRH::Edge::Edge()
{
}

TTRH::Edge::Edge(const Edge& other)
{
	m_fVer = other.m_fVer;
	m_sVer = other.m_sVer;
	m_tVer = other.m_tVer;
	m_arEdge = other.m_arEdge;
}

double TTRH::Edge::areaEdge(const AcGePoint3dArray& arEdge)
{
	m_fVer = arEdge.at(0);
	m_sVer = arEdge.at(1);
	m_tVer = arEdge.at(2);

	double area_x, area_y, area_z, area;
	
	area_x = ((m_sVer.y - m_fVer.y) * (m_tVer.z - m_fVer.z) - (m_sVer.z - m_fVer.z) * (m_tVer.y - m_fVer.y)) / 2.;
	area_y = ((m_sVer.x - m_fVer.x) * (m_tVer.z - m_fVer.z) - (m_sVer.z - m_fVer.z) * (m_tVer.x - m_fVer.x)) / 2.;
	area_z = ((m_sVer.x - m_fVer.x) * (m_tVer.y - m_fVer.y) - (m_sVer.y - m_fVer.y) * (m_tVer.x - m_fVer.x)) / 2.;

	area = std::sqrt( area_x*area_x + area_y*area_y + area_z*area_z );
			
	return area;
}

double TTRH::volumeTTRH(const AcGePoint3d& fVrt, const AcGePoint3d& sVrt, const AcGePoint3d& tVrt, const AcGePoint3d& ftVrt)
{
	AcGeVector3d vec_a, vec_b, vec_c;
	double volume, determinant;

	vec_a = sVrt - fVrt;
	vec_b = tVrt - fVrt;
	vec_c = ftVrt - fVrt;

	determinant = (vec_a.x*vec_b.y*vec_c.z+vec_a.y*vec_c.x*vec_b.z+vec_a.z*vec_b.x*vec_c.y) - (vec_a.z*vec_b.y*vec_c.x+vec_a.x*vec_c.y*vec_b.z+vec_b.x*vec_c.z*vec_a.y);

	volume = determinant / 6.0;

	return volume;
}