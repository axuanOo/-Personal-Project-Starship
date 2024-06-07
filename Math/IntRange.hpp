#pragma once

class IntRange
{
public:

	int					m_min;
	int					m_max;

	static const IntRange ZERO;
	static const IntRange ONE ;
	static const IntRange ZERO_TO_ONE;

public:

	IntRange();
	explicit IntRange(int minValue, int maxValue);
	~IntRange();


	bool const	IsOnRange( int value) const; 
	bool const	IsOverlappingWith( const IntRange& rangeValue ) const; 

	// Operators (const)
	bool		operator==(const IntRange& compare) const;		// FloatRange == FloatRange
	bool		operator!=(const IntRange& compare) const;		// FloatRange != FloatRange
	void		operator= (const IntRange& compare);			// FloatRange =  FloatRange

private:


};

