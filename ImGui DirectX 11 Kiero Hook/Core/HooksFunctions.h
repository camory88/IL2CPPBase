#pragma once

// includes
#include <string>
#include <intrin.h>

// my includes
#include <Libraries/Il2cpp_Resolver/il2cpp_resolver.hpp>

using namespace Variables;

struct BulletFlyby_Fields : UnityEngine_MonoBehaviour_Fields {
	struct PlayerController_o* playerController;
};
struct BulletFlyby_VTable {
	VirtualInvokeData _0_Equals;
	VirtualInvokeData _1_Finalize;
	VirtualInvokeData _2_GetHashCode;
	VirtualInvokeData _3_ToString;
};
struct BulletFlyby_c {
	Il2CppClass_1 _1;
	void* static_fields;
	Il2CppRGCTXData* rgctx_data;
	Il2CppClass_2 _2;
	BulletFlyby_VTable vtable;
};
struct BulletFlyby_o {
	BulletFlyby_c* klass;
	void* monitor;
	BulletFlyby_Fields fields;
};

struct MethodInfo_3A2990_RGCTXs {
	Il2CppType* _0_T;
};
struct MethodInfo_3A2990 {
	Il2CppMethodPointer methodPointer;
	void* invoker_method;
	const char* name;
	UnityEngine_Component_c* klass;
	const Il2CppType* return_type;
	const void* parameters;
	const MethodInfo_3A2990_RGCTXs* rgctx_data;
	union
	{
		const void* genericMethod;
		const void* genericContainer;
	};
	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint8_t parameters_count;
	uint8_t bitflags;
};

namespace HooksFunctions
{
	// EXAMPLE
	void(UNITY_CALLING_CONVENTION Health__TakeDamage)(UnityEngine_Component_o* __this, const MethodInfo_3A2990* method);
	void Health__TakeDamage_hook(UnityEngine_Component_o* __this, const MethodInfo_3A2990* method)
	{
		if (__this != nullptr) {

			//if(CheatMenuVariables::GodMode) _damage = 0;
		}

		printf("method called!\n");// "shot   x:%f y:%f z:%f\n", _______________________);


		return Health__TakeDamage(__this, method);
	}
}