#include<Core/Engine.h>
#include<Core/ScriptExporter.h>
#include<Windows.h>

class AKAME_SCRIPT Rotate : public Behaviour
{
private:
	float speed;
public:
	AK_SHOW_IN_INSPECTOR
		(
			AK_ID(speed)
		)
	Rotate()
	{
		speed = 10;
		std::cout <<"hello!\n";
	}

	~Rotate()
	{
		std::cout << "Bye!\n";
	}
    
	void Update(float del_time)override
	{
		Transform &transform=GetComponent<Transform>();
		transform.SetGlobalRotation(transform.GetGlobalRotation() * Quaternion::rotationAroundAxisVector(speed * del_time*20.0f ,glm::vec3(0,1,0) ));
	}
};
REGISTER_SCRIPT(Rotate);

class BackAndForth : public Behaviour
{
private:
	float speed;
	float time_curr;
public:
	AK_SHOW_IN_INSPECTOR
	(
		AK_ID(speed)
	)
		BackAndForth()
	{
		time_curr = 0.0f;
		speed = 1.5f;
	}

	void Update(float del_time)override
	{
		time_curr += del_time;
		Transform& transform = GetComponent<Transform>();
		transform.SetGlobalPosition(glm::vec3(0,0,sin(time_curr)*5.0f));
	}
};


REGISTER_SCRIPT(BackAndForth);