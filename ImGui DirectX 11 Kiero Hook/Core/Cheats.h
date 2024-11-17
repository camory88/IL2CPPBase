#pragma once

// my includes
#include <Libraries/Dumper.hpp>
#include <Libraries/imgui/imgui.h>
#include <Core/Variables.h>
#include <Utils/SDK.h>
#include <Utils/Themes.h>
#include <Utils/Utils.h>
#include <Core/InternalGameFunctions.h>

using namespace Variables;
static ImDrawList* ImGuiDrawAdd;

namespace Fns {

	bool RenderChams(Unity::CGameObject* obj) {
		// MAYBE THERE YOU SHOULD GET ANOTHER COMPONENT INSIDE THE CURRENT PLAYER AND AFTER FOUND THE RENDER LIST FROM THAT SUB GAMEOBJECT
		Unity::il2cppClass* CRenderer = IL2CPP::Class::Find("UnityEngine.Renderer");
		if (!CRenderer) return false;

		Unity::il2cppObject* CRSystemType = IL2CPP::Class::GetSystemType(CRenderer);
		if (!CRSystemType) return false;

		auto RenderList = obj->CallMethodSafe<Unity::il2cppArray<Unity::CComponent*>*>("GetComponentsInChildren", CRSystemType);
		if (!RenderList) return false;

		// THERE YOU SHOULD SKIP SOME RENDERERS, LIKE WEAPONS, OR OTHER STUFF

		for (int t = 0; t < RenderList->m_uMaxLength; t++)
		{
			if (!RenderList->operator[](t))
				continue;

			if (!RenderList->operator[](t)->GetPropertyValue<bool>("enabled"))
				continue;

			auto Material = RenderList->operator[](t)->CallMethodSafe<Unity::CComponent*>("GetMaterial");
			if (!Material)
				continue;

			Material->CallMethodSafe<void*>("set_shader", CheatVariables::ChamsShader);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_Cull"), 0);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_ZTest"), 8);
			Material->CallMethodSafe<void*>("SetInt", IL2CPP::String::New("_ZWrite"), 1);
			Material->SetPropertyValue<Unity::Color>("color", Unity::Color(CheatVariables::Rainbow.x, CheatVariables::Rainbow.y, CheatVariables::Rainbow.z, CheatVariables::Rainbow.w));

		}

		return true;
	}

	bool ExecAimbot(Unity::CGameObject* target, Vector2 playerHead) {

		if (CheatMenuVariables::AimbotFOVCheck)
		{
			if (playerHead.x > (System::ScreenCenter.x + CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.x < (System::ScreenCenter.x - CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.y > (System::ScreenCenter.y + CheatMenuVariables::AimbotFOV))
				return false;
			if (playerHead.y < (System::ScreenCenter.y - CheatMenuVariables::AimbotFOV))
				return false;
		}

		if (GetAsyncKeyState(VK_RBUTTON)) {
			if (CheatVariables::TargetPlayer == nullptr) {
				CheatVariables::TargetPlayer = target;
			}

			if (CheatVariables::TargetPlayer == target) {
				Utils::MouseMove(playerHead.x, playerHead.y, System::ScreenSize.x, System::ScreenSize.y, CheatMenuVariables::AimbotSmoothness);
			}
		}
		else {
			CheatVariables::TargetPlayer = nullptr;
		}

		return true;
	}

	bool Spinbot() {
		if (CheatMenuVariables::ShowMenu) return false;

		CheatVariables::SpinbotSupport = (CheatVariables::SpinbotSupport + 150) % 1000;

		Utils::MouseMove(CheatVariables::SpinbotSupport, System::ScreenSize.y, System::ScreenSize.x, System::ScreenSize.y, 0);

		return true;
	}
}


void DrawMenu()
{
	Themes::ImGuiThemeKio(true);

	if (CheatMenuVariables::ShowInspector) {
		Utils::DrawInspector();
	}

	if (ImGui::Begin(Prefix.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::SetWindowPos(ImVec2(500, 500), ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2(600, 300), ImGuiCond_Once);

		ImGui::Checkbox("Inspect", &Variables::CheatMenuVariables::ShowInspector);
		
		ImGui::Checkbox("PlayerChams", &CheatMenuVariables::PlayerChams);

		ImGui::Checkbox("Fov changer", &CheatMenuVariables::CameraFovChanger);
		if (CheatMenuVariables::CameraFovChanger)
		{
			ImGui::SliderFloat("Fov:", &CheatMenuVariables::CameraCustomFOV, 18, 180);
		}

		
		ImGui::Checkbox("Crosshair", &CheatMenuVariables::Crosshair);
		ImGui::Checkbox("draw AimbotFOV", &Variables::CheatMenuVariables::AimbotFOVCheck);
		ImGui::Checkbox("Aimbot", &Variables::CheatMenuVariables::EnableAimbot);
		if (CheatMenuVariables::CameraFovChanger)
		{
			ImGui::SliderFloat("Smoothness:", &CheatMenuVariables::AimbotSmoothness, 0, 20);
			ImGui::SliderFloat("Aimbot Fov:", &CheatMenuVariables::AimbotFOV, 36, 360);
		}
		ImGui::End();
	}
}

void CheatsLoop()
{
	DWORD currentTime = GetTickCount64(); 

	if (!Variables::System::InitIL2Cpp) return;
	if (CheatMenuVariables::CameraFovChanger)
	{
		Unity::CCamera* CameraMain = Unity::Camera::GetMain();
		if (CameraMain != nullptr) {
			CameraMain->CallMethodSafe<void*>("set_fieldOfView", CheatMenuVariables::CameraCustomFOV);
		}
	}

	if (CheatMenuVariables::EnableDeveloperOptions)	{

		for (int i = 0; i < CheatVariables::TestObjects::List.size(); i++) {
			Unity::CGameObject* curObject = CheatVariables::TestObjects::List[i];
			if (!curObject) continue;

			Unity::CTransform* objectTransform = curObject->GetTransform();
			if(!objectTransform) continue;

			Unity::Vector3 objectPos = objectTransform->GetPosition();

			Vector2 pos;
			if (!Utils::WorldToScreen(objectPos, pos)) continue;

			
				ImColor color = CheatVariables::TestObjects::SnaplineColor;

				ImGuiDrawAdd->AddLine({ 1920 / 2,1080 / 2 }, { pos.x,pos.y}, ImColor{20,20,222,255}, 2.f);
			

			if (CheatVariables::TestObjects::Chams) { 
				Fns::RenderChams(curObject);
			}

		}
	}

	for (int i = 0; i < CheatVariables::PlayersList.size(); i++)
	{
		Unity::CGameObject* curPlayer = CheatVariables::PlayersList[i];
		if (!curPlayer) continue;

		Unity::CTransform* playerTransform = curPlayer->GetTransform();
		if (!playerTransform) continue;

		Unity::Vector3 playerPos = playerTransform->GetPosition();
	
		Vector2 pos;
		if (!Utils::WorldToScreen(playerPos, pos)) continue;

		
		if (CheatMenuVariables::PlayerChams) { // ATTENTION: some game can crash keeping that in loop enabled
			Fns::RenderChams(curPlayer);
		}

		if (CheatMenuVariables::EnableAimbot)
		{
			if (check_in_fov(pos,CheatMenuVariables::AimbotFOV,1920,1080))
			{
				Fns::ExecAimbot(curPlayer, pos);
			}
		}


		ImColor color = CheatVariables::TestObjects::SnaplineColor;

		ImGui::GetBackgroundDrawList()->AddLine({1920 / 2,1080}, {pos.x,pos.y}, ImColor{20,20,222,255}, 2.f);
		
	}


	if (currentTime - CheatVariables::LastTick > 5)
	{
		CheatVariables::LastTick = currentTime;
	}
}

void CacheManager()
{
	for(;;)
	{
		if (!Variables::System::InitIL2Cpp)
			continue;

		void* m_pThisThread = IL2CPP::Thread::Attach(IL2CPP::Domain::Get());
		if (!m_pThisThread) continue;

		
		if (CheatMenuVariables::EnableDeveloperOptions) {
			Utils::ObjectsCache(&CheatVariables::TestObjects::List, CheatVariables::TestObjects::Name);
		}
			
		Utils::ObjectsCache(&CheatVariables::PlayersList, "UnityEngine.CapsuleCollider");
		
		

		IL2CPP::Thread::Detach(m_pThisThread);
		Sleep(10);
	}
}