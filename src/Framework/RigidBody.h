#ifndef __RigidBody_h_included__
#define __RigidBody_h_included__


class CRigidBody
{
	// weight

public:
	CRigidBody();

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() const { return m_radius; }

	void SetSpeed(const Vector3 & speed) { m_speed = speed; }
	const Vector3 & GetSpeed() const{ return m_speed; }

	float GetWeight() const { return m_weight; }
	void SetWeight(float weight) { m_weight = weight; }

private:
	Vector3 m_speed;
	float m_radius;
	float m_weight;
};


#endif __RigidBody_h_included__