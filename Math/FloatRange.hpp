#pragma once

class FloatRange
{
public:

	float					m_min;
	float					m_max;

	static const FloatRange ZERO;
	static const FloatRange ONE ;
	static const FloatRange ZERO_TO_ONE;

public:

	FloatRange();
	explicit FloatRange(float minValue, float maxValue);
	~FloatRange();


	bool const	IsOnRange( float value) const; 
	bool const	IsOverlappingWith(const FloatRange& rangeValue) const;
	bool const	IsInsideRange(const FloatRange& rangeValue) const;
	// Operators (const)
	bool		operator==(const FloatRange& compare) const;		// FloatRange == FloatRange
	bool		operator!=(const FloatRange& compare) const;		// FloatRange != FloatRange
	void		operator=(const FloatRange& compare);		// FloatRange =  FloatRange

private:


};

