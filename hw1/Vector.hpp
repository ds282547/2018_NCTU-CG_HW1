#ifndef _CG_HW1_VEC_H_
#define _CG_HW1_VEC_H_

struct Vector3f
{
public:
	float x, y, z;

	Vector3f()
	{
		x = y = z = 0;
	}

	Vector3f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// return first pointer of coord for glVertex3fv.
	float* operator() (unsigned offset = 0) {
		return &x + offset;
	}
	float& operator[] (unsigned offset) {
		return *( operator()(offset) );
	}
	
	void operator+= (Vector3f const &v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}
};

#endif // !_CG_HW1_VEC_H_
