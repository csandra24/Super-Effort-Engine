#include "../Globals.h"
#include "../Application.h"
#include "ModuleCamera.h"
#include "ModuleTimer.h"
#include "ModuleInput.h"
#include "ModuleDebugDraw.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "../lib/glew-2.1.0/include/GL/glew.h"
#include "../lib/SDL/include/SDL.h"
#include "SDL_scancode.h"
#include "../lib/MathGeoLib_Source/Math/float2.h"


ModuleCamera::ModuleCamera()
{
	frustum = new Frustum();
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	AVISO("Creating camera");

	aspectRatio = float(App->window->GetWidth()) / float(App->window->GetHeight());

	frustum->SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum->SetViewPlaneDistances(0.1f, 1000.0f);
	frustum->SetHorizontalFovAndAspectRatio(DEGTORAD * 90.0f, aspectRatio);

	frustum->SetPos(float3(0.0f, 3.0f, 10.0f));
	frustum->SetFront(-float3::unitZ);
	frustum->SetUp(float3::unitY);

	proj = frustum->ProjectionMatrix();
	view = frustum->ViewMatrix();

	return true;
}

update_status ModuleCamera::PreUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCamera::Update()
{
	float deltaTime = App->timer->GetDeltaTime();
	const float2& mouseMotion = App->input->GetMouseMotion();
	const float2& mouseWheel = App->input->GetMouseWheel();
	bool mouseWheelMove = App->input->GetMouseWheelMove();

	frustum->SetHorizontalFovAndAspectRatio(frustum->HorizontalFov(), aspectRatio);

	float finalmoveSpeed = movementSpeed;

	// WASD + QE
	if (App->input->getKey(SDL_SCANCODE_Q)) //Up
	{
		//AVISO("UP");
		frustum->SetPos(frustum->Pos() + frustum->Up().Normalized() * .1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_E)) //Down
	{
		
		frustum->SetPos(frustum->Pos() + frustum->Up().Normalized() * -.1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_W)) //Forward
	{
		
		frustum->SetPos(frustum->Pos() + frustum->Front() * 0.1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_S)) //Backward
	{
		
		frustum->SetPos(frustum->Pos() + frustum->Front() * -0.1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_A)) //Left
	{
		
		frustum->SetPos(frustum->Pos() + frustum->WorldRight() * -0.1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_D)) //Right
	{

		frustum->SetPos(frustum->Pos() + frustum->WorldRight() * 0.1f * finalmoveSpeed * deltaTime);

	}

	if (App->input->getKey(SDL_SCANCODE_F)) //Reset
	{
		frustum->SetPos(float3(0.0f, 3.0f, 10.0f));
		frustum->SetFront(-float3::unitZ);
		frustum->SetUp(float3::unitY);
	}

	if (App->input->getKey(SDL_SCANCODE_LSHIFT))
	{
		finalmoveSpeed *= 2;
	}


	// Rotation (Arrows)
	if (App->input->getKey(SDL_SCANCODE_UP))
	{
		frustum->SetFront(float3x3::RotateAxisAngle(frustum->WorldRight(), rotate * DEGTORAD * deltaTime) * frustum->Front().Normalized());
		frustum->SetUp(float3x3::RotateAxisAngle(frustum->WorldRight(), rotate * DEGTORAD * deltaTime) * frustum->Up().Normalized());
	}

	if (App->input->getKey(SDL_SCANCODE_DOWN))
	{
		frustum->SetFront(float3x3::RotateAxisAngle(frustum->WorldRight(), -rotate * DEGTORAD * deltaTime) * frustum->Front().Normalized());
		frustum->SetUp(float3x3::RotateAxisAngle(frustum->WorldRight(), -rotate * DEGTORAD * deltaTime) * frustum->Up().Normalized());
	}

	if (App->input->getKey(SDL_SCANCODE_LEFT))
	{
		frustum->SetFront(float3x3::RotateY(rotate * DEGTORAD * deltaTime) * frustum->Front().Normalized());
		frustum->SetUp(float3x3::RotateY(rotate * DEGTORAD * deltaTime) * frustum->Up().Normalized());
		frustum->WorldRight();
	}

	if (App->input->getKey(SDL_SCANCODE_RIGHT))
	{
		frustum->SetFront(float3x3::RotateY(-rotate * DEGTORAD * deltaTime) * frustum->Front().Normalized());
		frustum->SetUp(float3x3::RotateY(-rotate * DEGTORAD * deltaTime) * frustum->Up().Normalized());
		frustum->WorldRight();
	}

	//Move camera with mouse || I have tried to make the camera work better, but I can't :((
	if (App->input->GetMouseButton(1))
	{
		Rotate(float3x3::RotateY((mouseMotion.x * 0.1f) * rotate * DEGTORAD * deltaTime));
		Rotate(float3x3::RotateAxisAngle(frustum->WorldRight().Normalized(), (mouseMotion.y * 0.1f) * rotate * DEGTORAD * deltaTime));
	}

	// Zoom In and Out
	if (mouseWheelMove)
	{
		frustum->Front().Normalized()*( zoom* deltaTime* mouseWheel.y);
		App->input->SetMouseWheel(false);
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
    delete frustum;
	return true;
}

float4x4 ModuleCamera::GetProjectionMatrix() const 
{
	return frustum->ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix() const 
{
	return frustum->ViewMatrix();
}

void ModuleCamera::SetHorizontalFOV(float Fov)
{
	frustum->SetHorizontalFovAndAspectRatio(Fov, aspectRatio);
}

void ModuleCamera::SetVerticalFOV(float Fov)
{
	frustum->SetHorizontalFovAndAspectRatio(Fov, aspectRatio);
}

void ModuleCamera::SetPlaneDistance(float Near, float Far)
{
	frustum->SetViewPlaneDistances(Near, Far);
}

void ModuleCamera::SetAspectRatio(const float width, const float height)
{
	frustum->SetHorizontalFovAndAspectRatio(frustum->HorizontalFov(), width / height);
}

void ModuleCamera::LookAt(const float& x, const float& y, const float& z)
{
	frustum->SetFront(vec(x, y, z));
}

void ModuleCamera::WindowResized(unsigned width, unsigned height)
{
	frustum->SetHorizontalFovAndAspectRatio(frustum->HorizontalFov(), width / height);
}

void ModuleCamera::Rotate(const float3x3& rotationCamera)
{
	vec frontBack = frustum->Front().Normalized();
	frustum->SetFront(rotationCamera.MulDir(frontBack));
	vec UpDown = frustum->Up().Normalized();
	frustum->SetUp(rotationCamera.MulDir(UpDown));
}


