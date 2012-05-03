// XPathFinder.h: interface for the _XPathFinder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPATHFINDER_H_
#define _XPATHFINDER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

#define _XDEF_CHARACTERMOVESPEED_WALK					0.0022f
#define _XDEF_CHARACTERMOVESPEED_RUN					0.0042f
#define _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL1		0.00945f //0.0126f
#define _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL2		0.02f
#define _XDEF_CHARACTERMOVESPEED_HIPERRUN_LEVEL3		0.03f

#define _XDEF_RIDEOBBLIMITHEIGHT						0.87f

////////////////////////////////////////////////////////////////
// Path finder
#define MAX_GRIDSIZE	128

#define MIDPOINT_MAX	500
#define DIST_MULT		64

#define CLOSE_VAL		0.00000001
#define MAX_HDIST       10
#define MAX_HDIST_LONG  MAX_HDIST*DIST_MULT

#define MAXTILEDIST		(MAX_GRIDSIZE+8)
#define MAXTILE_XLEN	(MAXTILEDIST*3/2)
#define MAXTILE_YLEN	(MAXTILEDIST*3/2)
#define TILE_CENTER		(MAXTILE_XLEN/2)

const unsigned int	BLOCK_NORM =  DIST_MULT;
const unsigned int	BLOCK_DIAG = (int)(1.4142f * DIST_MULT);
const double		PI_VAL_2 = 2.0f * _X_PI;
const double		PI_VAL_2CLOSE = PI_VAL_2 - CLOSE_VAL;

class POINTSMALL
{
public:
    WORD x;
    WORD y;
    BYTE dir;
	
    inline POINTSMALL &operator =(const POINTSMALL &cval)   {x=cval.x; y=cval.y; dir = cval.dir; return *this;}
	//    inline POINTSMALL &operator =(const POINT &cval)   {x=(BYTE)cval.x; y=(BYTE)cval.y; return *this;}
    inline bool		operator ==(const POINTSMALL &cval)  {return (x==cval.x && y==cval.y  &&  dir==cval.dir);}
    inline bool		operator !=(const POINTSMALL &cval)  {return !(this->operator==(cval));}
	
};

inline POINTSMALL MAKESMALLPOINT(WORD xval, WORD yval, BYTE dir)
{
    POINTSMALL p;
    p.x		= xval;
    p.y		= yval;
    p.dir	= dir;
    return p;
}

struct FINFO
{
    D3DXVECTOR2 pos;
    double		 dir;
};

class _XFInfo : public _XItem
{
public:
	FINFO	Data;
};

struct LineSegment
{
    bool    bCircle;    // Is it a circle curve - T (or a straight line - F)
	
    D3DXVECTOR2  ptOrigin;   // For circles, the origin of the circle
    bool    bClockwise; // For circles, if the initial direction increases radians
    D3DXVECTOR2  ptStart;    // For straight lines, the starting point of this line
	
    double  radStart;   // The radians point at start of curve (or slope for straight line)
    double  radTotal;   // The total radians of the curve (if one)
    double  length;     // The total length of this line segment
};


struct SubPathNode
{
    DWORD   hitTable[4];    // 121 bit table (9x9) showing which tiles get hit
	//  when going from origin(4,4,d1) to dest(x,y,d2)
    float   length;         // Length of path going from origin to dest
};


// Macros for changing between angle segments (0-7) and true radian values
#define Radian2Segment(rad) ((int)(CapRadian(rad + _X_PI / 8.0) / (_X_PI / 4.0)))
#define Segment2Radian(seg) (((double)seg) * _X_PI / 4.0)



#pragma pack(push, 1)
// Define the structure to be used
struct PathTileNodeType {
	
    unsigned int  bOpen         : 1;  // Whether this node is in the "Open" list
    unsigned int  direction     : 3;  // Optional: Direction (0-7) at this node
    unsigned int  dummy         : 4;
    unsigned int  costFromStart : 24; // Lowest cost (calculated so far) from origin to			// 32 bit
	//  this tile
    unsigned int  totalCost     : 29; // Sum of cost to this point + heuristic est (* 64)		
	
    // X,Y coordinates of previous node on path													// 27
    unsigned int  parentDir     : 3;
    
	WORD          parentX;
    WORD          parentY;
	
    // If bOpen, then this tile belongs to an implicit "Open list", &
    //  prevOpen & nextOpen are the list pointers to neighboring elements
    POINTSMALL    prevOpen, nextOpen;
	
};

#pragma pack(pop)



class _XPathFinder  
{

public:	
	short				m_Heuristic[MAX_HDIST_LONG+1][8];

	// Define the static array of tile node info
	PathTileNodeType	m_PathTileNode[MAXTILE_XLEN][MAXTILE_YLEN][8];

	// Define array to hold sub-pathing table
	SubPathNode			m_SubPaths[8][48][8];

	// m_NextCheck array stores the ordering of a pre-computed list of nodes, for use
	//  in Smoothing pass (when bCheckList == true).
	short				m_CheckIndex[MAXTILE_XLEN][MAXTILE_YLEN];
	POINT				m_NextCheck[MIDPOINT_MAX+1];

	// For speed, we must have a separate array to tell if each node has been visited
	// (stores by individual bit)
	BYTE				m_bVisited[MAXTILE_XLEN*MAXTILE_YLEN];

	int					m_SqrtTable[MAXTILE_XLEN][MAXTILE_YLEN];

	POINT				m_DirOffset[48];
	BYTE				m_DirIndexFromOffset[7][7];


	// Setup param
	double				m_TurnRadius;						// turn radius of ship in grid units
	bool				m_bCurved;							// Whether to curve movement, or just use straight lines
	bool				m_bDirectional;  					// Whether pathing will formally check directions
	bool				m_bFixed;        					// Whether directions are fixed (16 total) or continuous
	bool				m_bSmooth48;     					// Whether to smooth the path afterwards: Directional-48
	bool				m_bSmoothSimple; 					//                                        Simple
	double				m_ShipWidth;     					// width of ship (relative to grid size)	
	double				m_Granularity;   					// How closely the blocking is tested along a path
	int					m_Search;        					// How many surrounding tiles to search
	BOOL				m_bIgnoreBounds; 					// Whether we should allow out-of-bounds searches
															//  (in this test program), to simulate realistic timing
	POINT				m_CollideArrayStartPosition;
public:
	_XPathFinder();
	virtual ~_XPathFinder();

	inline void SetVisited(int x, int y, int dir)
	{
		int offset = x*MAXTILE_YLEN + y;
		m_bVisited[offset] |= (1<<dir);
	}
	
	inline bool GetVisited(int x, int y, int dir)
	{
		int offset = x*MAXTILE_YLEN + y;
		return ( ((m_bVisited[offset] & (1<<dir))) ? true : false );
	}

	inline int DistanceMacro(POINT pta, POINT ptb)
	{
		return m_SqrtTable[(ptb.x > pta.x ? ptb.x - pta.x : pta.x - ptb.x)][(ptb.y > pta.y ? ptb.y - pta.y : pta.y - ptb.y)];
		//    return DistanceMacroOld(pta, ptb);
	}

	// Constrain a radian value to 0<=value<=2*PI
	inline double CapRadian(double r)
	{
		while (r >= 2.0*_X_PI)
			r -= 2.0*_X_PI;
		while (r < 0.0)
			r += 2.0*_X_PI;
		if (r < CLOSE_VAL  ||  r > PI_VAL_2CLOSE)
			return 0.0;
		return r;
	}

	inline short GetHeuristicDist(int dist, POINT ptOrigin, POINT ptDest, int dirToDest)
	{
		double angleCorrect = CapRadian(atan2(ptDest.y - ptOrigin.y, ptDest.x - ptOrigin.x));
		int dirdiff = (int) (AngleDiff(angleCorrect, Segment2Radian(dirToDest)) / (_X_PI / 8.0));
		dirdiff = min(7, dirdiff);
		
		if (dist <= MAX_HDIST_LONG) return m_Heuristic[dist][dirdiff];
		// if the points are more than 10 tiles apart, approximate distance
		//  using the heuristic for 10 tiles
		else return m_Heuristic[MAX_HDIST_LONG][dirdiff] + (dist - MAX_HDIST_LONG);
	}

	// Calculate the difference between two angles
	inline double AngleDiff (double angle1, double angle2)
	{
		// Calculate from both 0 and 180 degrees, and take minimum
		double calc1, calc2;
		calc1 = fabs(angle1-angle2);
		
		if (angle1 > _X_PI) angle1 -= 2*_X_PI;
		if (angle2 > _X_PI) angle2 -= 2*_X_PI;
		calc2 = fabs(angle1-angle2);
		return min(calc1, calc2);
	}
	BOOL   CheckGrid( int x, int y );
	int    FindPath(D3DXVECTOR2 ptOrigin, D3DXVECTOR2 ptDest, double startDirection, FINFO *pathArray);
	int    HybridPath(D3DXVECTOR2 ptOrigin, D3DXVECTOR2 ptDest, double startDirection, FINFO *pathArray);
	
	void   MergeAdjacent(POINT ptOrigin, FINFO *pathArray, int *pNumPoints);
	
	void   MakeStraightLine(D3DXVECTOR2 pGridSrc, D3DXVECTOR2 pGridDest, LineSegment *lineSeg);
	
	int    SmoothPath(FINFO *pathArray, int numPoints, D3DXVECTOR2 ptOrigin, double dirOrigin);
	
	int    ComputeRoute1(D3DXVECTOR2 pGridSrc, D3DXVECTOR2 pGridDest,
		double angleStart, LineSegment *lineSegment);
	int    ComputeRoute2(D3DXVECTOR2 pGridSrc, D3DXVECTOR2 pGridDest,
		double angleStart, double angleDest, LineSegment *lineSegment);
	
	void SetupSqrtTable();
	void SetupDirOffsets();
	void SetupHeuristicTable();
	void SetupTurnTable();
	
	
	// Internal pathing procedures
	int    DoThePath(D3DXVECTOR2 ptOrigin, D3DXVECTOR2 ptDest, double startDirection, FINFO *pathArray,
		POINT *pGridOffset, BYTE dirMaskEnd, bool bCheckList);
	int    ConstructPath(D3DXVECTOR2 ptOrigin, double startDirection, POINT ptDest, POINT gdOffset, FINFO *pathArray, int finalDir);
	bool   ComputeDirectRoute(D3DXVECTOR2 pGridSrc, D3DXVECTOR2 pGridDest, bool bClock,
		double angleStart, LineSegment *lineSeg);
	
	bool   CurveWalk(LineSegment *lineSeg);
	double FindTouchPoints(D3DXVECTOR2 ptOriginA, bool bClockwiseA, 
		D3DXVECTOR2 ptOriginB, bool bClockwiseB, double radius,
		double *pLineLength);
	bool   ComputeLineSegments(D3DXVECTOR2 pGridSrc, D3DXVECTOR2 pGridDest, bool bClockA, bool bClockB, 
		double angleStart, double angleStop, LineSegment *lineSeg);
	void   MakeLineSegmentsFixed(LineSegment *lineSeg);
	void   CalcVector (LineSegment *lineSeg, double dist, D3DXVECTOR2 *ptNow, double *pSlopeNow);
	void   CalcVectorFixed (LineSegment *lineSeg, double dist, D3DXVECTOR2 *ptNow, double *pSlopeNow);
	void   GetHitsAndLength(SubPathNode *node, D3DXVECTOR2 ptOrigin, double dirOrigin,
							D3DXVECTOR2 ptDest, double dirDest, double granularity);
	
};

#endif // !defined(_XPATHFINDER_H_)

