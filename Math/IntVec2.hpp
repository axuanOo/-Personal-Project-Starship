#pragma once

struct  IntVec2
{

public:
	int x = 0;
	int y = 0;

public:
	// Construction / Destruction
	~IntVec2();
	IntVec2();
	IntVec2( const IntVec2& copyFrom );
	explicit IntVec2( int initialX, int initialY );

	static const IntVec2 ZERO;

	// Accessors ( const methods )
	float				GetLength() const;
	int					GetTaxicabLength() const;
	int					GetLengthSquared() const;
	float				GetOrientationRadians() const;
	float				GetOrientationDegrees() const;

	IntVec2 const		GetRotated90Degrees() const;
	IntVec2 const		GetRotatedMinus90Degrees() const;

	// Mutators
	void				Rotate90Degrees();
	void				RotateMinus90Degrees();
	void				SetFromText(char const* text);

	// Operators
	bool				operator==(const IntVec2& compare) const;		// vec2 == vec2
	bool				operator!=(const IntVec2& compare) const;
	void				operator=( const IntVec2& copyFrom );
	IntVec2				operator-( const IntVec2& vecToSub ) const;
	IntVec2				operator+(const IntVec2& vecToAdd) const;
	void				operator+=(const IntVec2& vecToAdd);
};