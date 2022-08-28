#include "SceneCamera.h"
#include <CommonUtilities/UtilityFunctions.hpp>
#include <FGE/Rendering/Camera/Camera.h>
#include <FGE/Core/Application.h>

SceneCamera::SceneCamera()
{
	myInput = FGE::Application::Get().GetInput();

	myCamera = std::make_shared<FGE::Camera>();
	myCamera->SetPerspectiveProjection(90, {1280, 720}, 0.1, 100000.0f);

	myTransform.SetPosition(0,0,-100 );
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::Update(float aTimeStep)
{
	CU::Vector2f moveXZInput = { 0,0 };
	float moveYInput = 0;

	if (myInput->GetKey(Keys::W))
	{
		moveXZInput.y += 1.f;
	}
	if (myInput->GetKey(Keys::S))
	{
		moveXZInput.y -= 1.f;
	}
	if (myInput->GetKey(Keys::A))
	{
		moveXZInput.x -= 1.f;
	}
	if (myInput->GetKey(Keys::D))
	{
		moveXZInput.x += 1.f;
	}

	if (moveXZInput.LengthSqr() > 0)
	{
		moveXZInput.Normalize();
	}

	//go up if space is pressed, go down if ctrl is pressed
	if (myInput->GetKey(Keys::Space))
	{
		moveYInput += 1.f;
	}
	if (myInput->GetKey(Keys::Control))
	{
		moveYInput -= 1.f;
	}


	//Roatate camera with mouse delta
	if (myInput->GetKey(Keys::RightMouse))
	{

		CU::Vector3f cameraPos = myTransform.GetPosition();
		CU::Vector3f forward = myTransform.GetForward();
		CU::Vector3f right = myTransform.GetRight();
		CU::Vector3f up = myTransform.GetUp();

		cameraPos += forward * moveXZInput.y * aTimeStep * 500.f;
		cameraPos += right * moveXZInput.x * aTimeStep * 500.f;
		cameraPos += up * moveYInput * aTimeStep * 500.f;


		myTransform.SetPosition(cameraPos);


		CU::Vector2f mouseDelta = { static_cast<float>(myInput->GetMousePosDelta().x), static_cast<float>(myInput->GetMousePosDelta().y) };

		CU::Vector3f cameraRot = myTransform.GetRotation() + CU::Vector3f(mouseDelta.y, mouseDelta.x, 0) * 0.005f;

		//Clamp camera rotation to prevent camera from flipping over (degrees)
		cameraRot.x = CU::Clamp(cameraRot.x, -89.f, 89.f);


		myTransform.SetRotation(cameraRot);

	}
	
	myCamera->RecalculateViewMatrix(myTransform.GetMatrix());
}

void SceneCamera::Resize(unsigned int aWidth, unsigned int aHeight)
{
	myCamera->SetPerspectiveProjection(90, { aWidth, aHeight }, 0.1, 100000.0f);
	mySize = {static_cast<float>(aWidth), static_cast<float>(aHeight)};
}
