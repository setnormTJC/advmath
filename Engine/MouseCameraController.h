#pragma once
#include "Camera.h"
#include "Mouse.h"

class MouseCameraController
{
public:
	MouseCameraController( Mouse& mouse,Keyboard& kbd, Camera& cam )
		:
		mouse( mouse ),
		cam( cam ), 
		kbd(kbd)
	{}
	void Update(float dt)
	{

		if (kbd.KeyIsPressed('Q'))
		{
			cam.SetAngle(cam.GetAngle() + rotationSpeed * dt);
		}

		else if (kbd.KeyIsPressed('E')) //turn around and throw it in reverse
		{
			cam.SetAngle(cam.GetAngle() - rotationSpeed * dt);
		}

		while( !mouse.IsEmpty() )
		{
			const auto e = mouse.Read();
			switch( e.GetType() )
			{
			case Mouse::Event::Type::LPress:
				engaged = true;
				lastPos = (Vec2)e.GetPos();
				break;
			case Mouse::Event::Type::LRelease:
				engaged = false;
				break;
			case Mouse::Event::Type::WheelUp:
				cam.SetScale( cam.GetScale() * zoomFactor );
				break;
			case Mouse::Event::Type::WheelDown:
				cam.SetScale( cam.GetScale() / zoomFactor );
				break;
			}
		}

		if( engaged )
		{
			const auto curPos = (Vec2)mouse.GetPos();
			auto delta = curPos - lastPos;
			delta.x = -delta.x; // fixes the disconnect between screen coords and math coords


			delta.Rotate(-cam.GetAngle());

			cam.MoveBy( delta / cam.GetScale() );
			lastPos = curPos;
		}
	}
private:
	static constexpr float zoomFactor = 1.05f;

	static constexpr float rotationSpeed = 1.10f; 

	bool engaged = false;
	Vec2 lastPos;
	Mouse& mouse;
	Camera& cam;
	Keyboard& kbd;
};