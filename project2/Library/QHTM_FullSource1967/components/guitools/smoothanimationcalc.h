/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	SmoothAnimationCalc.h
Owner:	russf@gipsysoft.com
Purpose:	<Description of module>.
----------------------------------------------------------------------*/
#ifndef SMOOTHANIMATIONCALC_H
#define SMOOTHANIMATIONCALC_H

namespace GS
{
	class CSmoothAnimationCalc
	{
	public:
		CSmoothAnimationCalc( PI_uint32 uDivisor )
			: m_uDivisor( uDivisor )
			, m_uFrom( 0 )
			, m_uTo( 0 )
		{
			ASSERT( uDivisor );
		}

		void SetSizes( PI_uint32 uFrom, PI_uint32 uTo )
		{
			m_uFrom = uFrom;
			m_uTo = uTo;
		}

		PI_uint32 GetSize() const { return m_uFrom; }
		bool IsFinished() const { return m_uFrom == m_uTo; }
		void Increment()
		{
			if( m_uFrom < m_uTo )
			{
				if( m_uTo - m_uFrom <= m_uDivisor )
				{
					m_uFrom = m_uTo;
				}
				else
				{
					m_uFrom += ( m_uTo - m_uFrom ) / m_uDivisor;
				}
			}
			else
			{
				if( m_uFrom - m_uTo <= m_uDivisor )
				{
					m_uFrom = m_uTo;
				}
				else
				{
					m_uFrom -= ( m_uFrom - m_uTo ) / m_uDivisor;
				}
			}
		}

	private:
		PI_uint32 m_uDivisor;
		PI_uint32 m_uFrom;
		PI_uint32 m_uTo;
	};
};

#endif //SMOOTHANIMATIONCALC_H