#pragma once

struct  IntVec3
{

public:
	int x = 0;
	int y = 0;
	int z = 0;
public:
	// Construction / Destruction
	~IntVec3();
	IntVec3();
	IntVec3( const IntVec3& copyFrom );
	explicit IntVec3(int initialX, int initialY, int initialZ);

	static const IntVec3 ZERO;

	// Operators
	bool				operator==(const IntVec3& compare) const;		// vec2 == vec2
	bool				operator!=(const IntVec3& compare) const;
	void				operator=(const IntVec3& copyFrom);
	IntVec3				operator-(const IntVec3& vecToSub) const;
	IntVec3				operator+(const IntVec3& vecToAdd) const;
	void				operator+=(const IntVec3& vecToAdd);
};