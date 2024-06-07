#pragma once

class RandomNumberGenerator
{
public:
	int			RollRandomIntLessThan( int maxNotInclusive );
	int			RollRandomIntRange( int minInclusive, int maxInclusive );
	float		RollRandomFloatZeroToOne();
	float		RollRandomFloatInRange( float minInclusive, float maxInclusive );

private:
	//			unsigned int	m_seed = 0;
	//			int	m_position = 0;
};

